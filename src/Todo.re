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
  mutation ($id: ID!, $text: String!, $completed: Boolean!) {
    updateTodoSimple(id: $id, text: $text, completed: $completed) {
      id
      text
      completed
    }
  }
  |}
];
module UpdateTodoItem = ReasonApolloHooks.Mutation.Make(UpdateTodoItemConfig);

module DeleteTodoItemConfig = [%graphql
  {|
  mutation ($id: ID!) {
    deleteTodoSimple(id: $id) {
      deletedTodoItemId
    }
  }
|}
];
module DeleteTodoItem = ReasonApolloHooks.Mutation.Make(DeleteTodoItemConfig);

[@react.component]
let make = (~todo, ~refetchQueries) => {
  let completed =
    todo.completed->Belt.Option.mapWithDefault(false, value => value);

  let (updateTodoItem, _, _) = UpdateTodoItem.use();
  let (deleteTodoItem, _, _) = DeleteTodoItem.use();

  let handleUpdate = _ => {
    let variables =
      UpdateTodoItemConfig.make(
        ~id=todo.id,
        ~text=todo.text,
        ~completed=!completed,
        (),
      )##variables;

    updateTodoItem(~variables, ()) |> ignore;
  };

  let handleDelete = _ => {
    deleteTodoItem(
      ~variables=DeleteTodoItemConfig.make(~id=todo.id, ())##variables,
      ~refetchQueries=_ => refetchQueries,
      (),
    )
    |> ignore;
  };

  <li className={Cn.ifTrue("completed", completed)}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked=completed
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
