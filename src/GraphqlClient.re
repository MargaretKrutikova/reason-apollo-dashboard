let fragmentsData = [%bs.raw {|require('../fragmentTypes.json')|}];

let fragmentMatcher =
  ApolloInMemoryCache.createIntrospectionFragmentMatcher(~data=fragmentsData);

/* Create an InMemoryCache */
let inMemoryCache =
  ApolloInMemoryCache.createInMemoryCache(~fragmentMatcher, ());

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="http://localhost:4001/graphql", ());

let client =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());
