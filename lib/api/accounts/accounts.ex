defmodule Api.Accounts do
  @moduledoc """
  The Accounts context.
  """

  import Ecto.Query, warn: false

  alias Api.Repo
  alias Api.Accounts.User

  @doc """
  Returns the list of users.

  ## Examples

      iex> list_users()
      [%User{}, ...]

  """
  def list_users do
    Repo.all(User)
  end

  @doc """
  Gets a single user.

  Raises `Ecto.NoResultsError` if the User does not exist.

  ## Examples

      iex> get_user!(123)
      %User{}

      iex> get_user!(456)
      ** (Ecto.NoResultsError)

  """
  def get_user!(id), do: Repo.get!(User, id)

    @doc """
  Gets a single user.

  Returns nil if the User does not exist.

  ## Examples

      iex> get_user(123)
      %User{}

      iex> get_user(456)
      nil

  """
  def get_user(id), do: Repo.get(User, id)

  @doc """
  Gets a single user by name

  Returns nil if the User does not exist.

  ## Examples

      iex> get_user_by_name("good_value")
      %User{}

      iex> get_user_by_name("bad_value")
      nil

  """
  def get_user_by_name(name), do: Repo.get_by(User, name: name)

  @doc """
  Creates a user.

  ## Examples

      iex> create_user(%{field: value})
      {:ok, %User{}}

      iex> create_user(%{field: bad_value})
      {:error, %Ecto.Changeset{}}

  """
  def create_user(attrs \\ %{}) do
    %User{}
    |> User.registration_changeset(attrs)
    |> Repo.insert()
  end

  @doc """
  Updates a user.

  ## Examples

      iex> update_user(user, %{field: new_value})
      {:ok, %User{}}

      iex> update_user(user, %{field: bad_value})
      {:error, %Ecto.Changeset{}}

  """
  def update_user(%User{} = user, attrs) do
    user
    |> User.changeset(attrs)
    |> Repo.update()
  end

  @doc """
  Deletes a User.

  ## Examples

      iex> delete_user(user)
      {:ok, %User{}}

      iex> delete_user(user)
      {:error, %Ecto.Changeset{}}

  """
  def delete_user(%User{} = user) do
    Repo.delete(user)
  end

  @doc """
  Returns an `%Ecto.Changeset{}` for tracking user changes.

  ## Examples

      iex> change_user(user)
      %Ecto.Changeset{source: %User{}}

  """
  def change_user(%User{} = user) do
    User.changeset(user, %{})
  end

  @doc """
  Authenticate a User.

  ## Examples

      iex> authenticate(%{"name" => "name", "password" => "good_password"})
      true

      iex> authenticate(%{"name" => "name", "password" => "bad_password"})
      false

  """
  def authenticate(%{name: name, password: password}),
    do: authenticate(%{"name" => name, "password" => password})

  def authenticate(%{"name" => name, "password" => password}) do
    user = get_user_by_name(name)

    case check_password(user, password) do
      true -> {:ok, user}
      _ -> {:error, "Invalid username/password combination."}
    end
  end

  def authenticate(_), do: {:error, "Invalid username/password combination."}

  defp check_password(user, password) do
    case user do
      nil -> Bcrypt.no_user_verify()
      _ -> Bcrypt.verify_pass(password, user.password_hash)
    end
  end
end
