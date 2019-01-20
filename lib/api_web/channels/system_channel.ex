defmodule ApiWeb.SystemChannel do
  @moduledoc false

  use ApiWeb, :channel
  require Logger
  @name __MODULE__

  def join("system", _params, socket) do
    send(self(), :after_join)
    {:ok, socket}
  end

  def handle_info(:after_join, socket) do
    {:noreply, socket}
  end

  def terminate(reason, _socket) do
    log("#{@name} > leave #{inspect(reason)}")
    :ok
  end

  defp log(message), do: Logger.debug(fn -> message end)
end
