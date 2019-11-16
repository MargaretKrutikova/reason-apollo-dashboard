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

external castToTodos: Js.Json.t => AllTodosQuery.t = "%identity";

/** cache read/write modules for all todos */
module TodosReadQuery = ApolloClient.ReadQuery(AllTodosQuery);
module TodosWriteQuery = ApolloClient.WriteQuery(AllTodosQuery);

let readTodosFromCache = client => {
  let query = AllTodosQuery.make() |> toReadQueryOptions;

  switch (TodosReadQuery.readQuery(client, query)) {
  | exception _ => None
  | cachedTodos => cachedTodos->Js.Nullable.toOption
  };
};

let writeTodosToCache = (client, todos) => {
  TodosWriteQuery.make(~client, ~data={"allTodos": todos}, ());
};

let removeTodoFromCache = (client, todoId) => {
  let data = readTodosFromCache(client)->Belt.Option.map(castToTodos);
  switch (data) {
  | Some(todos) =>
    let updatedTodos =
      todos##allTodos->Belt.Array.keep(todo => todo##id != todoId);
    writeTodosToCache(client, updatedTodos);
  | None => ()
  };
};

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
  let canAddTodo = newTodoText != "" && addTodoResult != Loading;

  let handleAddTodo = () => {
    addTodo(
      ~variables=AddTodoMutation.make(~text=newTodoText, ())##variables,
      ~refetchQueries=_ => [|toQueryObj(AllTodosQuery.make())|],
      (),
    )
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
              ->Belt.Array.map(todo =>
                  <Todo key={todo##id} todo onDelete=removeTodoFromCache />
                )}
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
