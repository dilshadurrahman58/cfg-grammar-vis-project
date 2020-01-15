/*
    dot-util.cpp
*/

#include "dot-util.h"

#include<map>
#include<iostream>

namespace dot {


std::map<std::string, std::string>nodeToLabel;


std::string nodeEllipse(std::string id_str, std::string label) {
    nodeToLabel[id_str] = label;
    //std::cout<<id_str<<" "<<label<<std::endl;
    return id_str + " [label=\"" + label + "\"];\n\n";
}

std::string nodeDiamond(std::string id_str, std::string label) {
    //nodeToLabel[id_str] = label;
    //std::cout<< nodeToLabel[id_str]<<std::endl;
    return  id_str + " [shape=\"diamond\", margin=0, label=\""
            + label + "\"];\n\n";
}

std::string nodeWHead(std::string id_str, std::string label) {
    nodeToLabel[id_str] = label;
    //std::cout<< nodeToLabel[id_str]<<std::endl;
    return  id_str + " [shape=\"none\", margin=0, label = < "
            + "<table border=\"1\" cellborder=\"0\" cellspacing=\"0\" "
            + "cellpadding=\"4\">"
            + "<tr><td port=\"H\"></td></tr>"
            + "<tr><td>"+label+"</td></tr></table> > ];\n\n";
}

std::string nodeWLeftRight(std::string id_str, std::string label) {
    nodeToLabel[id_str] = label;
   //std::cout<< nodeToLabel[id_str]<<" "<<label<<std::endl;
    return  id_str + " [shape=\"none\", margin=0, label = < "
            +"<table border=\"1\" cellborder=\"0\" cellspacing=\"0\" "
            + "cellpadding=\"4\">"
            + "<tr><td colspan=\"2\">"+label
            +"</td></tr><tr><td port=\"L\"></td>"
            +"<td port=\"R\"></td></tr>"
            +"<tr><td port=\"head\"></td></tr></table> > ];\n\n";

            //std::cout<<"I am here"<<endl;
}

std::string nodeWFootList(std::string id_str, std::string label, int num) {
    nodeToLabel[id_str] = label;
  //std::cout<< nodeToLabel[id_str]<<" "<<label<<std::endl;
    std::string retval = id_str + " [shape=\"none\", margin=0, label = < "
            +"<table border=\"1\" cellborder=\"0\" cellspacing=\"0\" "
            + "cellpadding=\"4\">"
            + "<tr><td colspan=\"" + std::to_string(num) + "\">"+label
            +"</td></tr><tr>";
    for (int i=0; i<num; i++) {
        retval+= "<td port=\"F"+std::to_string(i)+"\"></td>";
    }
    retval += "</tr></table> > ];\n\n";
    return retval;
}

std::string nodeEllipseWFootList(std::string id_str, std::string label, int num) {
    nodeToLabel[id_str] = label;
    //std::cout<< nodeToLabel[id_str]<<" "<<label<<std::endl;
    //std::cout<<id_str<<" "<<label<<std::end;
    std::string retval = id_str + " [margin=0, label = < "
            +"<table border=\"0\" cellborder=\"0\" cellspacing=\"0\" "
            + "cellpadding=\"0\">"
            + "<tr><td colspan=\"" + std::to_string(num) + "\">"+label
            +"</td></tr><tr>";
    for (int i=0; i<num; i++) {
        retval+= "<td port=\"F"+std::to_string(i)+"\"></td>";
    }
    retval += "</tr></table> > ];\n\n";
    return retval;
}


std::string nodeWFoot(std::string id_str, std::string label) {
    nodeToLabel[id_str] = label;
    //std::cout<< nodeToLabel[id_str]<<" "<<label<<std::endl;

    return  id_str+" [shape=\"none\", margin=0, label = < "
            +"<table border=\"1\" cellborder=\"0\" cellspacing=\"0\" "
            +"cellpadding=\"4\">"
            +"<tr><td>"+label+"</td></tr>"
            +"<tr><td port=\"F\"></td></tr></table> > ];\n\n";
}

std::string from(std::string id)        { return id; }
std::string to(std::string id)          { return id; }
std::string fromLeft(std::string id)    { return id+":L"; }
std::string fromRight(std::string id)   { return id+":R"; }
std::string fromFoot(std::string id)    { return id+":F"; }
std::string fromFootList(std::string id, int whichItem)
    { return id+":F"+std::to_string(whichItem); }
std::string toHead(std::string id)      { return id+":H"; } 


std::string edgeString( std::string source, std::string target, 
                        std::string style) {

    //changing
    //std::cout<<nodeToLabel[source];
    //std::cout<<"I am here";
    //std::cout<<source;
    return source + " -> " + target + ";\n\n";
    //return nodeToLabel[source]+"_ID_"+source + " -> " + nodeToLabel[target]+"_ID_"+ target + " [style="+style+"];\n\n";
}

std::string edgeSolid(std::string source, std::string target) {
    return edgeString(source,target, "solid");
}

//modifying edgeDotted
std::string edgeDotted(std::string source, std::string target) {

   return "//"+ source + " -> " + target + " [style=dotted];\n\n";
}

/*std::string edgeDotted(std::string source, std::string target) {
    return edgeString(source,target,"dotted");
}*/

std::string edgeBold(std::string source, std::string target) {
    return edgeString(source,target,"bold");
}

}
