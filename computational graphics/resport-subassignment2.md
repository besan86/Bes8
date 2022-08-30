# 1 mesh vertices and faces output:

## Printing mesh model vertices into the stdout. 
## first picture shows the output, 
## second picture shows the code where it was written in the constructor (when we open file it gets loaded in utils::loadmeshmodel then the constructor is called -
## that way we make sure the vertices are printed).

![image](https://user-images.githubusercontent.com/75092336/101289254-ffa51b80-3803-11eb-808f-242edbc90cd6.png)

code:
![image](https://user-images.githubusercontent.com/75092336/101289320-61fe1c00-3804-11eb-9d92-41640985286b.png)


# 2 transformation 
## using showInMid Matrix was enough to make the mesh model fit in the screen.
## each vertix was x & y was multiplied by 500 and translated by half of the screen resolution).

![image](https://user-images.githubusercontent.com/75092336/101289402-e486db80-3804-11eb-939c-744df3504396.png)


# 3+4
## all model data were fetched from the scene.
## in the first loop we make sure we go through all the models in the scene.
## in the second loop we make sure for each model in the scene we go through its faces and apply transfromations before printing/drawing it to screen.


![image](https://user-images.githubusercontent.com/75092336/111044055-f1901800-844e-11eb-9824-c8e8ec13a7e1.png)
![image](https://user-images.githubusercontent.com/75092336/101289516-a9d17300-3805-11eb-8d8d-ff0c7cb8b6d5.png)

# 5
# traslation in local:
## X translated by 3.2

![image](https://user-images.githubusercontent.com/75092336/101289894-e9995a00-3807-11eb-86c8-1fc8f66828da.png)


# rotation in world after translation in local:
## rotated y by 55 degrees 
![image](https://user-images.githubusercontent.com/75092336/101289918-0cc40980-3808-11eb-8093-dbd94c533643.png)


# traslation in world:
# translated x by 4.2
![image](https://user-images.githubusercontent.com/75092336/101289824-6415aa00-3807-11eb-812c-b5e671c097ef.png)

# rotation in local after translation in world:
# rotated y by 255 degrees.
![image](https://user-images.githubusercontent.com/75092336/101289876-c1a9f680-3807-11eb-96cd-768e6d294a03.png)

# the difference between both Transformations is that after translation in both world and local is the rotation not effecting the postion of the model.
# (the local rotation rotated the model arount its self, in world rotation it was rotated around the axis).

# 6

# two models - "change active model" on "cow":
![image](https://user-images.githubusercontent.com/75092336/101290233-8e686700-3809-11eb-81f8-3fb5a06b236d.png)


## two models - "change active model"  on "beethoven":

![image](https://user-images.githubusercontent.com/75092336/101290215-7690e300-3809-11eb-8b1d-3db7cdce9c50.png)
