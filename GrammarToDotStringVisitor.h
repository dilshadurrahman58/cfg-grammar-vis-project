/*
    GrammarToDotStringVisitor.h

    Creates a dot string representation of a Grammar.
    
    usage:
        Grammar g;
        GrammarToDotStringVisitor strVisitor;
        g.acceptVisitor( strVisitor );
        std::string str = strVisitor.getString();
*/

#ifndef GrammarToDotStringVisitor_H_
#define GrammarToDotStringVisitor_H_

#include <map>
#include <list>
#include <set>

#include "GrammarVisitor.h"

namespace pwd {

class GrammarToDotStringVisitor : public GrammarVisitor {
    public:   
        GrammarToDotStringVisitor();
        ~GrammarToDotStringVisitor() {}

        std::string getString() { return mStr; }
       

        void outGrammar( Grammar* g,
            std::map<NonTerm,std::shared_ptr<SymLHS>> rules );

        void inRule( NonTerm lhs, std::shared_ptr<SymLHS> rule);
        
        void inEmpty( Empty* node );
        void outEmpty( Empty* node );

        void inEpsilon( Epsilon* node,
            std::list<std::shared_ptr<PTreeSym>> );
        void outEpsilon( Epsilon* node,
            std::list<std::shared_ptr<PTreeSym>> );

        void inSymLHS( SymLHS* node, NonTerm name,
            std::shared_ptr<GExp> rhs );
        void outSymLHS( SymLHS* node, NonTerm name,
            std::shared_ptr<GExp> rhs );

        void inSymRHS( SymRHS* node, NonTerm name, SymLHS* lhs );
        void outSymRHS( SymRHS* node, NonTerm name, SymLHS* lhs );

        void inTok( Tok* node, std::shared_ptr<Token> tok );
        void outTok( Tok* node, std::shared_ptr<Token> tok );

        void inAlt( Alt* node,
            std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2);
        void outAlt( Alt* node,
            std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2);

        void inSeq( Seq* node,
            std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2);
        void outSeq( Seq* node,
            std::shared_ptr<GExp> e1, std::shared_ptr<GExp> e2);

        void inDelta( Delta* node, std::shared_ptr<GExp> child);
        void outDelta( Delta* node, std::shared_ptr<GExp> child);

        void outRule( NonTerm lhs, std::shared_ptr<SymLHS> rule);

        std::map<std::string, std::string> nodeToDotID;




        //changing
        //prototype of the inRule and outRule functions
        //this functions are used to traverse for getting the clustered sections

    
    private:
        std::list<int>      mChildCount;    // for top alt and seq
        std::list<GExp*>    mCurrParent;    // Keeping track of current parent

        void pushParent(GExp* e);
        void popParent();
        GExp* getCurrParent();
        void incrChildCount();
        int getChildCount();

    private:
        int getNodeID(GExp*);   // returns a node id for given GExp
        std::string getNodeLabel(int);
        std::string parentToChild(GExp* child);
        std::string debugDecorate(std::string in_str, SymLHS* node);
        std::string debugDecorate(std::string, GExp*);
    
        std::string         mStr;           // whole dot string
        std::string         clusterStr;     //this string is for the clustering sections, it will be added to mStr 
        int cluster =0;

        std::string    nodeLabel;

        int                 mNodeCount;     // number of nodes to use for IDs

        std::map<GExp*,int> mNodeToID;
         //changing
        //std::map<int, std::string> nodeToLabel;  //map for int -> nodeLabel 

        //
        std::set<int>       mLHSconnections;
        
};

}
#endif
