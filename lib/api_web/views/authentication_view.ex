defmodule ApiWeb.AuthenticationView do
  @moduledoc false
  use ApiWeb, :view

  def render("show.json", %{token: token, user: user}), do: %{token: token, user: user_json(user)}

  def render("error.json", _), do: %{error: "Invalid username or password"}

  def render("refresh_error.json", _), do: %{error: "Invalid token"}

  def render("delete.json", _), do: %{ok: true}

  def render("forbidden.json", %{error: error}), do: %{error: error}

  # PRIVATE

  defp user_json(user), do: %{id: user.id, name: user.name}
end
