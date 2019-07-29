defmodule Api.Games.Player do
  @derive {Jason.Encoder, only: [
    :uuid, :id, :name, :model, :colour, :action,
    :x, :y, :z, :h, :pb
  ]}

  defstruct [
    uuid:   nil,
    id:     nil,
    name:   nil,
    model:  nil,
    colour: nil,
    action: nil,
    x:      nil,
    y:      nil,
    z:      nil,
    h:      nil,
    pb:     nil,
    status: :loading
  ]
end
