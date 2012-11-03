#include "structures/graph.h"
#include "structures/graphnode.h"
#include "structures/graphedge.h"

using namespace ohw::cgl;

void graphmain() {
  typedef Graph<GraphNode,GraphEdge,float> TGraph;
  TGraph graph;
  TGraph::TNode& node0 = graph.newNode();
  TGraph::TNode& node1 = graph.newNode();
  TGraph::TNode& node2 = graph.newNode();
  TGraph::TNode& node3 = graph.newNode();
  TGraph::TNode& node4 = graph.newNode();
  TGraph::TNode& node5 = graph.newNode();

  TGraph::TEdge& edge00 = graph.addEdge(node0,node1); edge00.setWeight(3);
  TGraph::TEdge& edge01 = graph.addEdge(node0,node2); edge01.setWeight(3);
  TGraph::TEdge& edge03 = graph.addEdge(node0,node3); edge03.setWeight(3);
  TGraph::TEdge& edge04 = graph.addEdge(node0,node4); edge04.setWeight(3);
  TGraph::TEdge& edge05 = graph.addEdge(node0,node5); edge05.setWeight(3);
  TGraph::TEdge& edge06 = graph.addEdge(node1,node2); edge06.setWeight(2);
  TGraph::TEdge& edge07 = graph.addEdge(node1,node5); edge07.setWeight(2);
  TGraph::TEdge& edge08 = graph.addEdge(node2,node3); edge08.setWeight(4);
  TGraph::TEdge& edge09 = graph.addEdge(node2,node4); edge09.setWeight(2);
  TGraph::TEdge& edge11 = graph.addEdge(node3,node4); edge11.setWeight(4);
  TGraph::TEdge& edge12 = graph.addEdge(node4,node5); edge12.setWeight(1);
  

  cout<<"grafo"<<endl;
  cout<<"node1: "<<node1<<endl;
  cout<<"node2: "<<node2<<endl;

  cout<<"isEdge(0,1): "<<cout<<graph.isEdge(node1,node2)<<endl;
  TGraph mst = TGraph::primsAlgorithm(graph,0);
  cout<<"mst"<<endl;
  for(unsigned int i=0; i<mst.getSizeOfNodes(); i++) {
    cout<<"- node "<<mst.getNode(i)<<": ";
    list<TGraph::TEdge*>& edges = mst.getEdges(mst.getNode(i));
    for(list<TGraph::TEdge*>::iterator j=edges.begin(); j!=edges.end(); j++) {
      cout<<"("<<*(*j)<<")";
    }
    cout<<endl;
  }
}
