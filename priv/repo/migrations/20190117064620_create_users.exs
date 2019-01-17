defmodule Api.Repo.Migrations.CreateUsers do
  use Ecto.Migration

  def change do
    create table(:users) do
      add :name, :citext
      add :email, :citext
      add :password_hash, :string

      timestamps()
    end
    create unique_index(:users, [:name])
    create unique_index(:users, [:email])
  end
end
