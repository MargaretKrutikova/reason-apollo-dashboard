open Types;

module Fragment = [%graphql
  {|
  fragment TodoItem on TodoItem @bsRecord {
    id
    text
    completed
  }
|}
];

module UpdateTodoItemConfig = [%graphql
  {|
  mutation ($input: UpdateTodoItemInput!) {
    updateTodoItem(input: $input) {
      updatedTodoItem {
        id
        text
        completed
      }
    }
  }
  |}
];
module UpdateTodoItem = ReasonApolloHooks.Mutation.Make(UpdateTodoItemConfig);

module DeleteTodoItemConfig = [%graphql
  {|
  mutation ($input: DeleteTodoItemInput!) {
    deleteTodoItem(input: $input) {
      deletedTodoItemId
    }
  }
|}
];
module DeleteTodoItem = ReasonApolloHooks.Mutation.Make(DeleteTodoItemConfig);

[@react.component]
let make = (~todo) => {
  let checked =
    todo.completed->Belt.Option.mapWithDefault(false, value => value);

  let (updateTodoItem, _, _) = UpdateTodoItem.use();
  let (deleteTodoItem, _, _) = DeleteTodoItem.use();

  let handleUpdate = _ => {
    let input = {
      "clientMutationId": None, // TODO: remove
      "id": todo.id,
      "text": todo.text,
      "completed": !checked,
    };

    updateTodoItem(
      ~variables=UpdateTodoItemConfig.make(~input, ())##variables,
      (),
    )
    |> ignore;
  };

  let handleDelete = _ => {
    let input = {
      "clientMutationId": None, // TODO: remove
      "id": todo.id,
    };

    deleteTodoItem(
      ~variables=DeleteTodoItemConfig.make(~input, ())##variables,
      (),
    )
    |> ignore;
  };

  <li className={Cn.ifTrue("completed", checked)}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked
          onChange=handleUpdate
        />
        {React.string(todo.text)}
      </label>
    </div>
    <i
      onClick=handleDelete
      role="button"
      className="remove mdi mdi-close-circle-outline"
    />
  </li>;
};
