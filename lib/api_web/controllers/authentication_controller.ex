defmodule ApiWeb.AuthenticationController do
  @moduledoc false
  use ApiWeb, :controller

  import ApiWeb.TokenHelpers

  alias Api.Accounts
  alias ApiWeb.FallbackController
  alias ApiWeb.Plug.EnsureAuthenticated

  action_fallback(FallbackController)

  # Uncomment for strict check!
  # plug(:scrub_params, "session" when action in [:create])

  plug(EnsureAuthenticated when action in [:refresh, :delete])

  def create(conn, %{"session" => session_params}) do
    with {:ok, user} <- Accounts.authenticate(session_params),
         token <- sign(user) do
      conn
      |> put_status(:created)
      |> render("show.json", token: token, user: user)
    end
  end
  def create(conn, _params) do
    conn
    |> put_status(:unauthorized)
    |> render("error.json")
  end

  def refresh(conn, _params) do
    with assigned_user <- Map.get(conn.assigns, :user),
        {:ok, user} <- load_user(assigned_user.id),
        token <- sign(user) do
      conn
      |> put_status(:created)
      |> render("show.json", token: token, user: user)
    end
  end

  def delete(conn, _) do
    conn |> render("delete.json")
  end

  defp load_user(id) do
    case Accounts.get_user(id) do
      nil -> {:error, "User not found"}
      user -> {:ok, user}
    end
  end
end
