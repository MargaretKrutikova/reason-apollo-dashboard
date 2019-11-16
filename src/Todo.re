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

[@react.component]
let make = (~todo) => {
  <li className={Cn.ifTrue("completed", todo.completed)}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          defaultChecked={todo.completed}
        />
        {React.string(todo.text)}
      </label>
    </div>
    <i role="button" className="remove mdi mdi-close-circle-outline" />
  </li>;
};
