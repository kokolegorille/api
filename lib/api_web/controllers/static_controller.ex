defmodule ApiWeb.StaticController do
  use ApiWeb, :controller

  def index(conn, _params) do
    conn
    |> put_resp_header("content-type", "text/html; charset=utf-8")
    |> send_file(200, "priv/static/index.html")
  end
end
