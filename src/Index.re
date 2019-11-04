[%bs.raw {|require("../assets/css/style.css")|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApolloHooks.ApolloProvider client=GraphqlClient.client>
    <Main />
  </ReasonApolloHooks.ApolloProvider>,
  "root",
);
