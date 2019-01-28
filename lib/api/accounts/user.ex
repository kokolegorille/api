defmodule Api.Accounts.User do
  use Ecto.Schema
  import Ecto.Changeset
  alias __MODULE__

  schema "users" do
    field :email, :string
    field :name, :string
    field :password, :string, virtual: true
    field :password_hash, :string

    timestamps()
  end

  @required_fields ~w(name email)a
  @registration_fields ~w(password)a
  @email_regex ~r/\A([^@\s]+)@((?:[-a-z0-9]+\.)+[a-z]{2,})\z/i

  @doc false
  def changeset(%User{} = user, attrs) do
    user
    |> cast(attrs, @required_fields)
    |> validate_required(@required_fields)
    |> validate_length(:name, min: 1, max: 32)
    |> validate_format(:email, @email_regex)
    |> unique_constraint(:name, message: "Name already taken")
    |> unique_constraint(:email, message: "Email already taken")
  end

  @doc false
  def registration_changeset(user, attrs \\ %{}) do
    user
    |> changeset(attrs)
    |> cast(attrs, @registration_fields)
    |> validate_required(@registration_fields)
    |> validate_length(:password, min: 6, max: 32)
    |> generate_encrypted_password()
  end

  # PRIVATE

  defp generate_encrypted_password(changeset) do
    case changeset do
      %Ecto.Changeset{valid?: true, changes: %{password: password}} ->
        put_change(changeset, :password_hash, Bcrypt.hash_pwd_salt(password))
      _ ->
        changeset
    end
  end
end
