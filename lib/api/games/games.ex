defmodule Api.Games do
  alias Api.Games.{Player, Worker, WorkerSup}
  alias ApiWeb.Notifier
  require Logger

  def get_worker(name) do
    # https://elixirforum.com/t/how-to-check-globally-named-agent/24056/2
    case WorkerSup.start_worker(name) do
      {:ok, worker} -> worker
      {:error, {:already_started, worker}} -> worker
      _response -> nil
    end
  end

  defdelegate get_world(game), to: Worker

  defdelegate get_player(game, uuid), to: Worker

  defdelegate get_world_diff(game, previous_state), to: Worker

  def join(game, uuid) do
    Worker.join(game, %Player{uuid: uuid})
  end

  def leave(game, uuid) do
    Worker.leave(game, %Player{uuid: uuid})
  end

  def ready(game, uuid) do
    Worker.ready(game, %Player{uuid: uuid})
  end

  def update(game, uuid, %{
    "id" => id,
    "model" => model, "name" => name, "colour" => colour,
    "action" => action,
    "h" => h, "pb" => pb, "x" => x, "y" => y, "z" => z
  }) do
    data = [
      id: id,
      name: name,
      model: model, colour: colour,
      action: action,
      h: h, pb: pb, x: x, y: y, z: z
    ]
    Worker.update(game, uuid, data)
  end

  def update(game, uuid, %{
    "action" => action,
    "h" => h, "pb" => pb, "x" => x, "y" => y, "z" => z
  }) do
    data = [
      action: action,
      h: h, pb: pb, x: x, y: y, z: z
    ]
    Worker.update(game, uuid, data)
  end

  def update(_, _, payload) do
    Logger.debug(fn -> "UPDATE ERROR #{inspect payload}" end)
  end

  def notify(message), do: Notifier.notify(message)
end
