/*
    GrammarToDotStringVisitor.cpp
    
    Mapping each node to its string representation so parent nodes
    can grab a child's string representation and include it.
*/

#include <iostream>
#include <sstream>

#include "GrammarToDotStringVisitor.h"
#include "GrammarToDotRulesVisitor.h"
#include "../PTreeVisitors/PTreeToDotStringVisitor.h"
#include "../dot-util.h"
#include "../explicit-pwd.h"

namespace pwd {

GrammarToDotStringVisitor::GrammarToDotStringVisitor() : mNodeCount(0) {}

void GrammarToDotStringVisitor::pushParent(GExp* e) {
    mCurrParent.push_front(e);
    mChildCount.push_front(0);
}
void GrammarToDotStringVisitor::popParent() {
    mCurrParent.pop_front();
    mChildCount.pop_front();
}
GExp* GrammarToDotStringVisitor::getCurrParent()   {
    if (mCurrParent.empty()) {
        return NULL;
    } else {
        return mCurrParent.front();
    }
}
void GrammarToDotStringVisitor::incrChildCount() {
    mChildCount.front()++;
}
int GrammarToDotStringVisitor::getChildCount() {
    return mChildCount.front();
}

int GrammarToDotStringVisitor::getNodeID(GExp* node) {
    if (mNodeToID.find(node) == mNodeToID.end()) {
        mNodeToID[node] = mNodeCount++;
        //std::cout<<node<<" "<<mNodeToID[node]<<std::endl;
    }
    return mNodeToID[node];
}

//changing
/*std::string GrammarToDotStringVisitor::getNodeLabel(int nodeID) {
   return nodeToLabel[nodeID];
}*/


// Dot string for edge from parent to given child.
std::string GrammarToDotStringVisitor::parentToChild(GExp* child) {
    assert(child!=NULL);

    // Get strings for parent and child ids
    std::string parent_id_str = std::to_string(getNodeID(getCurrParent()));
    std::string field_str = dot::fromFootList(parent_id_str,getChildCount());
    std::string child_id_str = std::to_string(getNodeID(child));
    incrChildCount();

    //std::string parent_node = getCurrParent();
 
    // Seq will have a list of children
    if (getCurrParent()!=NULL) {
        if (getCurrParent()->isaSeq()) {
            return dot::edgeSolid(dot::from(field_str), dot::to(child_id_str));

        // Otherwise, just assume parent is nodeWFoot
        } else {
            
            return dot::edgeSolid(dot::fromFoot(parent_id_str), 
                                  dot::to(child_id_str));
        }
    }
    
    return "";
}

// appends debug information such as the node address to given string
std::string
GrammarToDotStringVisitor::debugDecorate(std::string in_str, SymLHS* node) {
    std::string retval = debugDecorate(in_str, (GExp*)node);
    DEBUG_CODE({
        std::stringstream nullable;
        nullable << node->isNullable();
        retval += " nullable=" + nullable.str();
    })
    return retval;
}
std::string
GrammarToDotStringVisitor::debugDecorate(std::string in_str, GExp* node) {
    std::string node_str;
    DEBUG_CODE({
        std::stringstream address;
        address << std::hex << node;
        node_str += " (" + address.str() + ")";
    })
    return in_str + node_str;
}

// assuming that mStr has all of the strings for the rules
void GrammarToDotStringVisitor::outGrammar( Grammar* g,
            std::map<NonTerm,std::shared_ptr<SymLHS>> rules ) {
    std::string dot_string = "digraph {\n";
    
    // Grammar rules in text in a single box.
    GrammarToDotRulesVisitor visitor;
    g->acceptVisitor(visitor);
    dot_string += visitor.getString();
    
    // Grammar "tree" should be in mStr
    dot_string += mStr;
    
    mStr = dot_string + "}\n";
}

void GrammarToDotStringVisitor::inRule( NonTerm lhs, std::shared_ptr<SymLHS> rule)
{
    cluster++;
    clusterStr = "subgraph cluster_"+std::to_string(cluster)+"{ \nlabel = \"Cluster_"+std::to_string(cluster)+"\";\n node[shape = record];\n ";
}



// Empty node
void GrammarToDotStringVisitor::inEmpty( Empty* node ) {
    mStr += parentToChild(node);
}
void GrammarToDotStringVisitor::outEmpty( Empty* node ) {
    int node_id = getNodeID(node);
    nodeToDotID[std::to_string(node_id)] = "Empty_ID_" + std::to_string(node_id);

    mStr += dot::nodeWHead(nodeToDotID[std::to_string(node_id)], "Empty");
    //otherStr += label + ";"

    //nodeToLabel[node_id] = "Empty";

    //std::cout<<nodeToLabel[node_id]<<" "<<node_id<<std::endl;

//adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";



}

// Epsilon node
void GrammarToDotStringVisitor::inEpsilon( Epsilon* node,
            std::list<std::shared_ptr<PTreeSym>> ptree_list) {
    mStr += parentToChild(node);
}
void GrammarToDotStringVisitor::outEpsilon( Epsilon* node,
            std::list<std::shared_ptr<PTreeSym>> ptree_list) {
    int node_id = getNodeID(node);

  

    // Generate dot for PTrees
    for (const auto& ptree : ptree_list) {
        // FIXME: assuming top PTree node is going to have mCount as ID
        mStr += dot::edgeBold( dot::fromFoot(std::to_string(node_id)),
                               dot::to(std::to_string(mNodeCount)) );     
        PTreeToDotStringVisitor ptree_visitor(mNodeCount);
        ptree->acceptVisitor(ptree_visitor);
        mStr += ptree_visitor.getString();
        mNodeCount = ptree_visitor.getCurrCount();
    }

    nodeToDotID[std::to_string(node_id)] = debugDecorate("Epsilon",node) + "_ID_" + std::to_string(node_id);

    // dot for Epsilon node and edge into it
    mStr += dot::nodeWFoot(nodeToDotID[std::to_string(node_id)], 
                           debugDecorate("Epsilon",node));
    //changing
    //nodeToLabel[node_id]= "Epsilon";

    //adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";
}

// SymLHS
void GrammarToDotStringVisitor::inSymLHS( SymLHS* node, NonTerm name,
        std::shared_ptr<GExp> rhs ) {
    pushParent(node);
}
void GrammarToDotStringVisitor::outSymLHS( SymLHS* node, NonTerm name,
        std::shared_ptr<GExp> rhs ) {
    int node_id = getNodeID(node); 

    nodeToDotID[std::to_string(node_id)] = debugDecorate(name.toString(),node) + "_ID_" + std::to_string(node_id);

    mStr += dot::nodeWFoot(nodeToDotID[std::to_string(node_id)],
                           debugDecorate(name.toString(),node));
    //changing
    //nodeToLabel[node_id] = "lhs";

 //adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";

    popParent();
}

// SymRHS
void GrammarToDotStringVisitor::inSymRHS( SymRHS* node, NonTerm name,
        SymLHS* lhs ) {
    mStr += parentToChild(node);
}
void GrammarToDotStringVisitor::outSymRHS( SymRHS* node, NonTerm name,
        SymLHS* lhs ) {
    int node_id = getNodeID(node);
    nodeToDotID[std::to_string(node_id)] = debugDecorate(name.toString(),node) + "_ID_" + std::to_string(node_id);
    // own node
    mStr += dot::nodeWHead( nodeToDotID[std::to_string(node_id)],
                            debugDecorate(name.toString(),node));

    // dotted edge to SymLHS
    int lhsID = getNodeID(lhs);
    // FIXME: maybe a flag to turn on and off?
    // Only put the edge from the SymRHS to the SymLHS tree once
    // to avoid obscuring the output when a non-terminal shows up often.
    //if (mLHSconnections.find(lhsID)==mLHSconnections.end()) {
        mStr += dot::edgeDotted(dot::from(std::to_string(node_id)), 
                                dot::to(std::to_string(lhsID)));
        mLHSconnections.insert(lhsID);

        //changing
        //nodeToLabel[node_id] = "rhs";
    //}

  //adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";

}

// Tok
void GrammarToDotStringVisitor::inTok( Tok* node, std::shared_ptr<Token> tok ) {
    mStr += parentToChild(node);
}
void GrammarToDotStringVisitor::outTok(Tok* node, std::shared_ptr<Token> tok ) {
    int node_id = getNodeID(node);

    nodeToDotID[std::to_string(node_id)] = debugDecorate(tok->toString(),node) + "_ID_" + std::to_string(node_id);
    mStr += dot::nodeDiamond(nodeToDotID[std::to_string(node_id)],
                             debugDecorate(tok->toString(),node));
    //nodeToLabel[node_id] = "tok";
    //adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";

}

// Alt
void GrammarToDotStringVisitor::inAlt( Alt* node,
        std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2 ) {
    // if top Alt node
    if (!getCurrParent()->isaAlt()) {
        mStr += parentToChild(node);
        pushParent(node);
    }
}
void GrammarToDotStringVisitor::outAlt( Alt* node,
        std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2 ) {
    if (getCurrParent()==node) {
        int node_id = getNodeID(node);

        nodeToDotID[std::to_string(node_id)] = debugDecorate("alt", node) + "_ID_" + std::to_string(node_id);

        mStr += dot::nodeWFoot( nodeToDotID[std::to_string(node_id)],
                                debugDecorate("alt", node));
        //changing
        //nodeToLabel[node_id] = "alt";
        //adding nodes for cluster
        clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";
        popParent();
    }
}

// Seq
void GrammarToDotStringVisitor::inSeq( Seq* node,
        std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2 ) {
    // If top Seq node
    if (!getCurrParent()->isaSeq()) {
        mStr += parentToChild(node);
        pushParent(node);
    }
}
void GrammarToDotStringVisitor::outSeq( Seq* node,
        std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2 ) { 
    if (getCurrParent()==node) {
        int node_id = getNodeID(node);
        nodeToDotID[std::to_string(node_id)] = debugDecorate("seq", node) + "_ID_" + std::to_string(node_id);


        mStr += dot::nodeWFootList( nodeToDotID[std::to_string(node_id)], 
                                    debugDecorate("seq", node), 
                                    getChildCount());
        //changing
        //nodeToLabel[node_id] = "seq";
        //adding nodes for cluster
        clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";
        popParent();
    }
}

// Delta
void GrammarToDotStringVisitor::inDelta( Delta* node,
    std::shared_ptr<GExp> child ) {
    mStr += parentToChild(node);
    pushParent(node);
}
void GrammarToDotStringVisitor::outDelta( Delta* node,
        std::shared_ptr<GExp> child ) {
    int node_id = getNodeID(node);

    nodeToDotID[std::to_string(node_id)] = debugDecorate("delta", node) + "_ID_" + std::to_string(node_id);


    mStr += dot::nodeWFoot(nodeToDotID[std::to_string(node_id)], 
                           debugDecorate("delta",node));
    //nodeToLabel[node_id] = "delta";
    //adding nodes for cluster
    clusterStr += nodeToDotID[std::to_string(node_id)] + "; ";
    popParent();
}

void GrammarToDotStringVisitor::outRule(NonTerm lhs, std::shared_ptr<SymLHS> rule)
{
    clusterStr += "}\n\n";
    mStr += clusterStr;

    
}



}