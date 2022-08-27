# Wolfram Mathematica (wolframscript)
# Setup
PacletDataRebuild[]
<< SetReplace`

initgraph = {{0,1},{2,1},{1,1},{2,2},{2,2}};
rule = {{x,a},{y,a},{y,y},{y,y}} -> {{x,a},{y,a},{y,b},{z,b},{b,b},{y,y},{z,z},{z,z}};
Export["rule.png", RulePlot[WolframModel[rule],VertexLabels -> Automatic]]

g = ResourceFunction["WolframModel"][rule,initgraph,10,"FinalState"];
Export["grid-start.png", ResourceFunction["WolframModel"][rule,initgraph,10,"FinalStatePlot"]];

rule2 = {{a,x},{b,x},{b,y},{c,y},{b,b}} -> {{a,x},{b,x},{b,y},{c,y},{x,i},{y,i},{i,i}};
rule3 = {{b,x},{b,y}, {b,b}} -> {{b,x},{b,y},{x,i},{y,i},{i,i}};

Export["rule3.png",RulePlot[WolframModel[rule3],VertexLabels -> Automatic]]

hg = WolframModel[rule3,g,15];

graph = hg["CausalGraph"];
graphlarge = Show[graph, ImageSize -> 2000];
Export["graph.png", graphlarge];
