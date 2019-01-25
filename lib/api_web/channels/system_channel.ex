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

  # def handle_info(:after_join, socket) do
  #   push(socket, "presence_state", Presence.list(socket))
  #   {:ok, _} = Presence.track(socket, socket.assigns.user_id, %{
  #     online_at: System.system_time(:second)
  #   })
  #   push(socket, "ping", %{ping_time: :os.system_time(:millisecond)})
  #   {:noreply, socket}
  # end

  def handle_info(:after_join, socket) do
    push(socket, "presence_state", Presence.list(socket))
    {:ok, _} = Presence.track(socket, 1, %{
      online_at: System.system_time(:second)
    })
    push(socket, "ping", %{ping_time: :os.system_time(:millisecond)})
    {:noreply, socket}
  end

  # Calculate lag for a given user
  def handle_in("pong", payload, socket) do
    user_id = socket.assigns.user_id

    case payload["ping_time"] do
      ping_time when is_number(ping_time) ->
        lag = :os.system_time(:millisecond) - ping_time
        latency = %{user_id: user_id, lag: lag, ping_time: ping_time}
        log("> user : #{user_id}, lag : #{latency.lag}")

      _ ->
        log("> user : #{user_id}, lag : undefined")
    end

    {:noreply, socket}
  end

  def terminate(reason, _socket) do
    log("#{@name} > leave #{inspect(reason)}")
    :ok
  end

  # PRIVATE

  defp log(message), do: Logger.debug(fn -> message end)
end
