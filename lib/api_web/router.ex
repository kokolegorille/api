defmodule ApiWeb.Router do
  use ApiWeb, :router

  pipeline :api do
    plug :accepts, ["json"]
  end

  pipeline :api_auth do
    plug(ApiWeb.Plug.VerifyHeader, realm: "Bearer")
  end

  scope "/api", ApiWeb do
    pipe_through :api

    scope "/v1" do
      post("/registrations", RegistrationController, :create)
      post("/authentication", AuthenticationController, :create)

      resources "/users", UserController, except: [:new, :edit]

      # Secure API
      pipe_through(:api_auth)
      patch("/authentication/refresh", AuthenticationController, :refresh)
      delete("/authentication", AuthenticationController, :delete)
    end
  end

  scope "/", ApiWeb do
    get "/*path", StaticController, :index
  end
end
