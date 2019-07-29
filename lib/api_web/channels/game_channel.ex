defmodule ApiWeb.GameChannel do
  @moduledoc false
  use ApiWeb, :channel
  require Logger
  @name __MODULE__
  @refresh_time_in_ms 30
  alias ApiWeb.Presence
  alias Api.Games
  alias Api.Games.GameManager

  def join("game:" <> id, _params, socket) do
    log("Connected to game #{id}")

    # Unique socket identifier
    uuid = UUID.uuid4()
    socket = assign(socket, :uuid, uuid)

    id
    |> String.to_integer()
    |> Games.get_worker()
    |> Games.join(uuid)

    GameManager.monitor_channel(self(), {id, uuid})

    Process.send_after(self(), {:tick, []}, @refresh_time_in_ms)

    send(self(), :after_join)
    {:ok, socket}
  end

  def handle_in("init", payload, socket) do
    "game:" <> id = socket.topic
    worker = id
    |> String.to_integer()
    |> Games.get_worker()

    Games.update(worker, socket.assigns.uuid, payload)
    world = Games.get_world(worker)
    push(socket, "world_init", %{world: world})
    {:noreply, socket}
  end

  def handle_in("player_ready", _payload, socket) do
    "game:" <> id = socket.topic
    uuid = socket.assigns.uuid

    worker = id
    |> String.to_integer()
    |> Games.get_worker()

    Games.ready(worker, uuid)
    player = Games.get_player(worker, uuid)
    broadcast_from!(socket, "game_joined", %{player: player})

    {:noreply, socket}
  end

  def handle_in("update", payload, socket) do
    "game:" <> id = socket.topic
    id
    |> String.to_integer()
    |> Games.get_worker()
    |> Games.update(socket.assigns.uuid, payload)

    {:noreply, socket}
  end

  def handle_in(command, payload, socket) do
    message = "#{@name} > Unknown command '#{command}' " <>
      "(#{inspect(command, base: :hex)}) " <>
      "with payload #{inspect(payload)}"

    Logger.debug fn -> message end
    {:noreply, socket}
  end

  def handle_info(:after_join, socket) do
    uuid = socket.assigns.uuid
    push(socket, "uuid_init", %{uuid: uuid})

    push(socket, "presence_state", Presence.list(socket))
    {:ok, _} = Presence.track(socket, socket.assigns.user.id, %{
      uuid: uuid,
      name: socket.assigns.user.name,
      online_at: System.system_time(:second)
    })
    {:noreply, socket}
  end

  def handle_info({:tick, previous_state}, socket) do
    "game:" <> id = socket.topic

    worker = id
    |> String.to_integer()
    |> Games.get_worker()

    world = Games.get_world(worker)
    diff = get_diff(world, previous_state)

    if length(diff) > 0,
      do: broadcast!(socket, "world_update", %{world: diff})

    Process.send_after(self(), {:tick, world}, @refresh_time_in_ms)
    {:noreply, socket}
  end

  def terminate(reason, _socket) do
    log("#{@name} > leave #{inspect(reason)}")
    :ok
  end

  # PRIVATE

  defp log(message), do: Logger.debug(fn -> message end)

  defp get_diff(world, previous) do
    previous_map = previous
    |> Enum.map(fn p -> {p.id, p} end)
    |> Enum.into(%{})

    # Filter player's event that were Idle, or the same as in
    # the previous state except for in place action!

    # in_place_actions = ["Pointing", "Pointing Gesture", "Belly Dance"]
    in_place_actions = []

    Enum.filter(world, fn player ->
      player.action in in_place_actions ||
      player != Map.get(previous_map, player.id)
    end)
  end
end
