defmodule ApiWeb.TokenHelpers do
  @moduledoc """
  Hold logic for signing and checking phoenix token
  """

  alias ApiWeb.Endpoint

  @salt "user salt"
  # one week
  @max_age 7 * 24 * 3600

  @spec sign(term) :: String.t()
  def sign(user), do: Phoenix.Token.sign(Endpoint, @salt, user)

  @spec verify_token(String.t()) :: {:ok, integer} | {:error, term}
  def verify_token(token), do: Phoenix.Token.verify(Endpoint, @salt, token, max_age: @max_age)
end
