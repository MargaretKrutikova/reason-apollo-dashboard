[%bs.raw {|require("../assets/css/style.css")|}];
[%bs.raw {|require("../assets/vendors/mdi/css/materialdesignicons.min.css")|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApolloHooks.ApolloProvider client=GraphqlClient.client>
    <Main />
  </ReasonApolloHooks.ApolloProvider>,
  "root",
);
