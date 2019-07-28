defmodule ApiWeb.SystemChannel do
  @moduledoc false

  use ApiWeb, :channel
  require Logger
  @name __MODULE__
  alias ApiWeb.Presence

  def join("system", _params, socket) do
    send(self(), :after_join)
    {:ok, socket}
  end

  def handle_info(:after_join, socket) do
    push(socket, "presence_state", Presence.list(socket))
    {:ok, _} = Presence.track(socket, socket.assigns.user.id, %{
      name: socket.assigns.user.name,
      online_at: System.system_time(:second)
    })
    push(socket, "ping", %{ping_time: :os.system_time(:millisecond)})
    {:noreply, socket}
  end

  # Calculate lag for a given user
  def handle_in("pong", payload, socket) do
    user = socket.assigns.user

    case payload["ping_time"] do
      ping_time when is_number(ping_time) ->
        lag = :os.system_time(:millisecond) - ping_time
        latency = %{user: user, lag: lag, ping_time: ping_time}
        log("> user : #{user.id}, lag : #{latency.lag}")

      _ ->
        log("> user : #{user.id}, lag : undefined")
    end

    {:noreply, socket}
  end

  # For testing
  def handle_in("ping", payload, socket) do
    user = socket.assigns.user
    timestamp = :os.system_time(:millisecond)
    push(socket, "pong", %{ping_time: timestamp})

    case payload["ping_time"] do
      ping_time when is_number(ping_time) ->
        lag = timestamp - ping_time
        latency = %{user: user, lag: lag, ping_time: ping_time}
        log("> user : #{user.id}, lag : #{latency.lag}")

      _ ->
        log("> user : #{user.id}, lag : undefined")
    end

    {:noreply, socket}
  end

  def handle_in(command, payload, socket) do
    log("#{command} #{inspect payload}")
    {:noreply, socket}
  end

  def terminate(reason, _socket) do
    log("#{@name} > leave #{inspect(reason)}")
    :ok
  end

  # PRIVATE

  defp log(message), do: Logger.debug(fn -> message end)
end
