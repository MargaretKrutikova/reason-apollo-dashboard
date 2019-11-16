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

[@react.component]
let make = () => {
  let (todosResult, _) = useQuery((module AllTodosQuery));

  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form onSubmit={e => ReactEvent.Form.preventDefault(e)}>
        <div className="add-items d-flex">
          <input
            type_="text"
            className="form-control todo-list-input"
            placeholder="What do you need to do today?"
          />
          <button
            type_="submit"
            className="add btn btn-gradient-primary font-weight-bold todo-list-add-btn"
            id="add-task">
            {React.string("Add")}
          </button>
        </div>
      </form>
      <div className="list-wrapper">
        <ul className="d-flex flex-column-reverse todo-list todo-list-custom">
          {switch (todosResult) {
           | Loading => <Spinner />
           | Data(data) =>
             {data##allTodos
              ->Belt.Array.map(todo => <Todo key={todo.id} todo />)}
             |> React.array
           | Error(_) =>
             <div> {"Ooops! En error occured." |> React.string} </div>
           | NoData => React.null
           }}
        </ul>
      </div>
    </div>
  </div>;
};
