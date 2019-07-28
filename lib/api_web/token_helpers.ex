defmodule ApiWeb.TokenHelpers do
  @moduledoc """
  Hold logic for signing and checking phoenix token
  """

  alias ApiWeb.Endpoint

  @salt "user salt"
  # one week
  @max_age 7 * 24 * 3600

  # This is used in authentication plug and user_socket
  def sign(user), do: Phoenix.Token.sign(Endpoint, @salt, %{id: user.id, name: user.name})

  def verify_token(token), do: Phoenix.Token.verify(Endpoint, @salt, token, max_age: @max_age)
end
