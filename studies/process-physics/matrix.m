geo = Table[0, {i, 1000}, {j, 1000}] ;

Do[ Do[newGeo[i,j] = 1 / geo[i,j], {i,1000}] ,{j, 1000}]
