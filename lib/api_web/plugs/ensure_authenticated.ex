defmodule ApiWeb.Plug.EnsureAuthenticated do
  @moduledoc false

  import Plug.Conn
  import ApiWeb.TokenHelpers

  def init(opts \\ []) do
    opts
  end

  def call(conn, _opts) do
    token = conn.assigns[:token]

    with {:ok, user_id} <- verify_token(token) do
      conn |> assign(:user_id, user_id)
    else
      {:error, _reason} ->
        conn
        |> put_status(:unauthorized)
        |> put_resp_content_type("application/json")
        |> send_resp(401, Jason.encode!(%{error: "Unauthorized"}))
        |> halt()
    end
  end
end
