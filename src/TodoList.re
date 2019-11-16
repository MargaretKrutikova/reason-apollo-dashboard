open ApolloHooks;

module AllTodosQuery = [%graphql
  {|
  query {
    allTodos {
      ...Todo.Fragment.TodoItem
    }
  }
|}
];

module AddTodoMutation = [%graphql
  {|
  mutation ($text: String!) {
    addTodoSimple(text: $text) {
      id
    }
  }
|}
];

[@react.component]
let make = () => {
  let (todosResult, _) = useQuery((module AllTodosQuery));

  let (addTodo, addTodoResult, _) = useMutation((module AddTodoMutation));

  let (newTodoText, setNewTodoText) = React.useState(() => "");

  let canAddTodo =
    switch (addTodoResult) {
    | Loading => false
    | _ => true
    };

  let handleAddTodo = () => {
    let variables = AddTodoMutation.make(~text=newTodoText, ())##variables;

    let refetchQueries = _ => [|toQueryObj(AllTodosQuery.make())|];

    addTodo(~variables, ~refetchQueries, ())
    |> Js.Promise.(then_(_ => setNewTodoText(_ => "") |> resolve))
    |> ignore;
  };

  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form
        onSubmit={e => {
          ReactEvent.Form.preventDefault(e);
          handleAddTodo();
        }}>
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
            disabled={!canAddTodo}>
            {React.string("Add")}
          </button>
        </div>
      </form>
      <div className="list-wrapper">
        <ul className="d-flex flex-column-reverse todo-list todo-list-custom">
          {switch (todosResult) {
           | Data(data) =>
             {data##allTodos
              ->Belt.Array.map(todo => <Todo key={todo.id} todo />)}
             |> React.array
           | Loading => <Spinner />
           | Error(_) =>
             <div> {"Ooops! En error occured." |> React.string} </div>
           | NoData => React.null
           }}
        </ul>
      </div>
    </div>
  </div>;
};
