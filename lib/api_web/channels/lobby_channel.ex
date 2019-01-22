defmodule ApiWeb.LobbyChannel do
  @moduledoc false
  use ApiWeb, :channel
  require Logger
  @name __MODULE__

  def join("lobby", _params, socket) do
    send(self(), :after_join)
    {:ok, socket}
  end

  ## HANDLE_INFO
  def handle_info(:after_join, socket) do
    {:noreply, socket}
  end

  def terminate(reason, _socket) do
    log("#{@name} > leave #{inspect(reason)}")
    :ok
  end

  # PRIVATE

  defp log(message), do: Logger.debug(fn -> message end)
end
