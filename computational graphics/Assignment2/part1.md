# triangle fill 
## used the barycentric approach to fill the triangles

## for each pixel/point we check if its in the current triangle (using its distance from each edge(vector) and the triangle space).
![image](https://user-images.githubusercontent.com/75092336/111044709-dcb58380-8452-11eb-8e9d-11956c5f5d6c.png)
![image](https://user-images.githubusercontent.com/75092336/111044725-f35bda80-8452-11eb-892c-425569995572.png)

![image](https://user-images.githubusercontent.com/75092336/105396228-4fab3680-5c28-11eb-9496-b2588a47885b.png)
## artifacts because no depth buffer was implemented yet.
![image](https://user-images.githubusercontent.com/75092336/105396268-5b96f880-5c28-11eb-9b43-7fccd1a9ce28.png)



# gray scale

## z was calculated using the barycentric coeffiecent calculated in the triangle fill process

![image](https://user-images.githubusercontent.com/75092336/105397810-31463a80-5c2a-11eb-9033-fa86870e0553.png)

# code

![image](https://user-images.githubusercontent.com/75092336/105397860-4622ce00-5c2a-11eb-9212-4261fc3e78d6.png)
