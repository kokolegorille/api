defmodule ApiWeb.FallbackController do
  use ApiWeb, :controller

  alias ApiWeb.{ChangesetView, ErrorView}

  def call(conn, {:error, %Ecto.Changeset{} = changeset}) do
    conn
    |> put_status(:unprocessable_entity)
    |> put_view(ChangesetView)
    |> render("error.json", changeset: changeset)
  end

  def call(conn, {:error, :not_found}) do
    conn
    |> put_status(:not_found)
    |> put_view(ErrorView)
    |> render(:"404")
  end

  def call(conn, {:error, reason}) do
    conn
    |> put_status(:not_found)
    |> put_view(ErrorView)
    |> render("error_with_reason.json", reason: reason)
  end
end
