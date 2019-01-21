defmodule ApiWeb.StaticController do
  use ApiWeb, :controller

  def index(conn, _params) do
    file = "priv/static/index.html"
    {:ok, binary} = File.read(file)
    html(conn, binary)
  end
end
