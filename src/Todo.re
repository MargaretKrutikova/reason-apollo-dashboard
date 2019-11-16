open ApolloHooks;
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
let make = (~todo: todo, ~refetchQueries) => {
  let (updateTodoItem, _, _) = useMutation((module UpdateTodoMutation));
  let (deleteTodoItem, _, _) = useMutation((module DeleteTodoMutation));

  let handleUpdate = _ => {
    let variables =
      UpdateTodoMutation.make(
        ~id=todo.id,
        ~text=todo.text,
        ~completed=!todo.completed,
        (),
      )##variables;

    updateTodoItem(~variables, ()) |> ignore;
  };

  let handleDelete = _ => {
    deleteTodoItem(
      ~variables=DeleteTodoMutation.make(~id=todo.id, ())##variables,
      ~refetchQueries,
      (),
    )
    |> ignore;
  };

  <li className={Cn.ifTrue("completed", todo.completed)}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked={todo.completed}
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
