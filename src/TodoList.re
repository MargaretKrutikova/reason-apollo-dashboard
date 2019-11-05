module TodoQueryConfig = [%graphql
  {|
  {
	todos(limit: 20, offset:0) {
    ...Todo.Fragment.TodoItem
  }
}
|}
];
module TodoQuery = ReasonApolloHooks.Query.Make(TodoQueryConfig);

module AddTodoConfig = [%graphql
  {|
  mutation ($input: AddTodoItemInput!) {
    addTodoItem(input: $input) {
      addedTodoItem {
        ...Todo.Fragment.TodoItem
      }
    }
  }
|}
];

module AddTodoMutation = ReasonApolloHooks.Mutation.Make(AddTodoConfig);

[@react.component]
let make = () => {
  let (simple, _) = TodoQuery.use();
  let (addTodo, addTodoStatus, _) = AddTodoMutation.use();

  let (newTodoText, setNewTodoText) = React.useState(() => "");

  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form onSubmit={e => ReactEvent.Form.preventDefault(e)}>
        <div className="add-items d-flex">
          <input
            type_="text"
            className="form-control todo-list-input"
            placeholder="What do you need to do today?"
            value=newTodoText
            onChange={event => {
              let value = ReactEvent.Form.currentTarget(event)##value;
              setNewTodoText(_ => value);
            }}
          />
          <button
            type_="submit"
            className="add btn btn-gradient-primary font-weight-bold todo-list-add-btn"
            id="add-task"
            disabled={
              switch (newTodoText, addTodoStatus) {
              | ("", _)
              | (_, Loading) => true
              | _ => false
              }
            }>
            {React.string("Add")}
          </button>
        </div>
      </form>
      <div className="list-wrapper">
        <ul className="d-flex flex-column-reverse todo-list todo-list-custom">
          {switch (simple) {
           | Data(data) =>
             {data##todos
              ->Belt.Array.map(todo =>
                  switch (todo) {
                  | Some(todo) => <Todo todo />
                  | _ => React.null
                  }
                )}
             |> React.array
           | _ => <div> {"Not clear" |> React.string} </div>
           }}
        </ul>
      </div>
    </div>
  </div>;
};
