# Apparently, static is a reserved word!

# == Compilation error in file lib/api_web/router.ex ==
# ** (ArgumentError) `static` is a reserved route prefix generated from StaticController or `:as` option
#     (phoenix) lib/phoenix/router/scope.ex:31: Phoenix.Router.Scope.route/8
#     lib/api_web/router.ex:29: (module)
#     (stdlib) erl_eval.erl:680: :erl_eval.do_apply/6
#     (elixir) lib/kernel/parallel_compiler.ex:229: anonymous fn/4 in Kernel.ParallelCompiler.spawn_workers/7

# Rename to MyStatic... instead

defmodule ApiWeb.MyStaticController do
  use ApiWeb, :controller

  def index(conn, _params) do
    file = "priv/static/index.html"
    {:ok, binary} = File.read(file)
    html(conn, binary)
  end
end
