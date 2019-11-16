open ApolloHooks;

module Fragment = [%graphql
  {|
  fragment TodoItem on TodoItem {
    id
    text
    completed
  }
|}
];

module UpdateTodoMutation = [%graphql
  {|
  mutation ($id: ID!, $text: String!, $completed: Boolean!) {
    updateTodoSimple(id: $id, text: $text, completed: $completed) {
      ...Fragment.TodoItem
    }
  }
  |}
];

module DeleteTodoMutation = [%graphql
  {|
  mutation ($id: ID!) {
    deleteTodoSimple(id: $id) {
      deletedTodoItemId
    }
  }
|}
];

[@react.component]
let make = (~todo, ~onDelete) => {
  let (updateTodoItem, _, _) = useMutation((module UpdateTodoMutation));
  let (deleteTodoItem, _, _) =
    useMutation(
      ~update=
        (cache, result: Mutation.mutationResult(DeleteTodoMutation.t)) => {
          let idToDelete =
            result##data##deleteTodoSimple
            ->Belt.Option.map(data => data##deletedTodoItemId);

          switch (idToDelete) {
          | None => ()
          | Some(id) => onDelete(cache, id)
          };
        },
      (module DeleteTodoMutation),
    );

  let handleUpdate = _ => {
    let variables =
      UpdateTodoMutation.make(
        ~id=todo##id,
        ~text=todo##text,
        ~completed=!todo##completed,
        (),
      )##variables;

    updateTodoItem(~variables, ()) |> ignore;
  };

  let handleDelete = _ => {
    deleteTodoItem(
      ~variables=DeleteTodoMutation.make(~id=todo##id, ())##variables,
      (),
    )
    |> ignore;
  };

  <li className={Cn.ifTrue("completed", todo##completed)}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked=todo##completed
          onChange=handleUpdate
        />
        {React.string(todo##text)}
      </label>
    </div>
    <i
      onClick=handleDelete
      role="button"
      className="remove mdi mdi-close-circle-outline"
    />
  </li>;
};
