defmodule ApiWeb.RegistrationController do
  @moduledoc false
  use ApiWeb, :controller

  import ApiWeb.TokenHelpers, only: [sign: 1]

  alias Api.Accounts
  alias ApiWeb.{AuthenticationView, FallbackController}

  action_fallback(FallbackController)

  plug(:scrub_params, "user" when action in [:create])

  def create(conn, %{"user" => params}) do
    with {:ok, user} <- Accounts.create_user(params),
         token <- sign(user) do
      conn
      |> put_status(:created)
      |> put_view(AuthenticationView)
      |> render("show.json", token: token, user: user)
    end
  end
end
