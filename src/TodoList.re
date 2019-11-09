module AllTodosConfig = [%graphql
  {|
  query {
    allTodos {
      ...Todo.Fragment.TodoItem
    }
  }
|}
];
module AllTodosQuery = ReasonApolloHooks.Query.Make(AllTodosConfig);

module AddTodoConfig = [%graphql
  {|
  mutation ($text: String!) {
    addTodoSimple(text: $text) {
      ...Todo.Fragment.TodoItem
    }
  }
|}
];
module AddTodoMutation = ReasonApolloHooks.Mutation.Make(AddTodoConfig);

[@react.component]
let make = () => {
  let (todosResult, _) = AllTodosQuery.use();
  let (addTodo, addTodoStatus, _) = AddTodoMutation.use();

  let (newTodoText, setNewTodoText) = React.useState(() => "");
  let canAdd = newTodoText != "" && addTodoStatus != Loading;

  let refetchQueries = [|
    ReasonApolloHooks.Utils.toQueryObj(AllTodosConfig.make()),
  |];
  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form
        onSubmit={e => {
          ReactEvent.Form.preventDefault(e);

          addTodo(
            ~variables=AddTodoConfig.make(~text=newTodoText, ())##variables,
            ~refetchQueries=_ => refetchQueries,
            (),
          )
          |> Js.Promise.(then_(_ => setNewTodoText(_ => "") |> resolve))
          |> ignore;
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
            disabled={!canAdd}>
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
                  <Todo key={todo.id} todo refetchQueries />
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
