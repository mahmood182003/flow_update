using namespace boost;
using namespace std;

enum FlowID {
	BLUE, RED
};

// define custom property names
enum Usage_E {
	flow_old, flow_new, flow_both, flow_none
};

struct Flow {
	Flow() :
			usage(flow_none) {
	}
	Flow(Usage_E usage) :
			usage(usage) {
	}
	Usage_E usage;
};

struct Edge_label {
	Edge_label() :
			capacity(1) {
	}
	int capacity;
	Flow flows[2];
};
struct graph_label {
	int id;
};

template<class Graph>
using Edge= typename graph_traits<Graph>::edge_descriptor;
template<class Graph>
using Vertex = typename graph_traits<Graph>::vertex_descriptor;

struct myTypes {
	// create a typedef for the Graph type
	typedef property<edge_index_t, int, Edge_label> myEdgeProp_t;
	typedef property<vertex_name_t, string> myVertexProp_t;
	typedef property<graph_name_t, int> myGraphProp_t;

	typedef adjacency_list<vecS, vecS, bidirectionalS,
			property<vertex_color_t, default_color_type, myVertexProp_t>,
			myEdgeProp_t, myGraphProp_t> DiGraph;

	typedef subgraph<DiGraph> MyGraph;
	typedef std::vector<Vertex<MyGraph>> VertexList;

	typedef adjacency_list<vecS, vecS, bidirectionalS,
			property<vertex_distance_t, size_t>> DAG;

	typedef pair<bool, size_t> Result;
};

template<class Graph>
void print_network(const Graph& G) {
	typedef typename boost::graph_traits<Graph>::vertex_iterator Viter;
	typedef typename boost::graph_traits<Graph>::out_edge_iterator OutEdgeIter;
	typedef typename boost::graph_traits<Graph>::in_edge_iterator InEdgeIter;

	int fid = get_property(G, graph_name);
	Viter ui, uiend;
	tie(ui, uiend) = vertices(G);

	for (; ui != uiend; ++ui) {
		OutEdgeIter out, out_end;
		cout << get(vertex_name, G.root())[G.local_to_global(*ui)] << "\t";

		tie(out, out_end) = out_edges(*ui, G);
		for (; out != out_end; ++out)
			cout << "--("

//					<< count_if(begin(G[*out].flows), end(G[*out].flows),
//							[](Flow f) {return f.usage!=flow_none;}) << ','
					<< ((G[*out].flows[fid].usage == flow_old) ? "old" : "new")
					<< "/" << G[*out].capacity << ")--> "
					<< get(vertex_name, G.root())[G.local_to_global(
							target(*out, G))] << "\t";

		InEdgeIter in, in_end;
		cout << endl << "\t";
		tie(in, in_end) = in_edges(*ui, G);
//		for (; in != in_end; ++in)
//			cout << "<--(" << G[*in].flows[0].ID << "/" << G[*in].capacity
//					<< ")-- " << source(*in, G) << "\t";

		cout << endl;
	}
}

// Make convenient labels for the vertices
struct Example {
	int S;
	int T;
	int U;
	int V;
	int W;
	int numNodes = 5;
} example;

void exampleNetwork(myTypes::MyGraph &g, myTypes::MyGraph &pair1,
		myTypes::MyGraph &pair2) {
	put(vertex_name, g, example.S = add_vertex(g), "S");
	put(vertex_name, g, example.T = add_vertex(g), "T");
	put(vertex_name, g, example.W = add_vertex(g), "W");
	put(vertex_name, g, example.U = add_vertex(g), "U");
	put(vertex_name, g, example.V = add_vertex(g), "V");

	Edge<myTypes::MyGraph> e;
	e = add_edge(example.U, example.W, g).first;
	g[e].capacity = 1;
	g[e].flows[BLUE] = Flow(flow_new);

	e = add_edge(example.W, example.V, g).first;
	g[e].capacity = 1;
	g[e].flows[BLUE] = Flow(flow_new);

	e = add_edge(example.S, example.W, g).first;
	g[e].capacity = 1;
	g[e].flows[BLUE] = Flow(flow_old);
	g[e].flows[RED] = Flow(flow_new);

	e = add_edge(example.S, example.U, g).first;
	g[e].capacity = 2;
	g[e].flows[RED] = Flow(flow_old);
	g[e].flows[BLUE] = Flow(flow_new);

	e = add_edge(example.W, example.T, g).first;
	g[e].capacity = 2;
	g[e].flows[BLUE] = Flow(flow_old);
	g[e].flows[RED] = Flow(flow_new);

	e = add_edge(example.U, example.V, g).first;
	g[e].capacity = 1;
	g[e].flows[RED] = Flow(flow_old);

	e = add_edge(example.V, example.T, g).first;
	g[e].capacity = 1;
	g[e].flows[RED] = Flow(flow_old);
	g[e].flows[BLUE] = Flow(flow_new);
	add_vertex(example.S, pair1);
	add_vertex(example.T, pair1);
	add_vertex(example.W, pair1);
	add_vertex(example.U, pair1);
	add_vertex(example.V, pair1);

	add_vertex(example.S, pair2);
	add_vertex(example.T, pair2);
	add_vertex(example.W, pair2);
	add_vertex(example.U, pair2);
	add_vertex(example.V, pair2);

}
