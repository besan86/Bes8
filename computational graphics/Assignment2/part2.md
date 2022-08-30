# 4. light and shading demo:
# light Code:
![image](https://user-images.githubusercontent.com/75092336/111078995-b0f0d700-8500-11eb-8e38-9ebbc1dbb3a7.png)

## basic lighting at pos 0,1,0.5 flat shading : 

![image](https://user-images.githubusercontent.com/75092336/111078732-a71aa400-84ff-11eb-9084-8f8b083e98a0.png)
 
 ## light translation to 1,0,0 ( means now its to the right):
 ![image](https://user-images.githubusercontent.com/75092336/111078758-c31e4580-84ff-11eb-94f4-7bd5ba6e537c.png)

## rotation around Y axis ~180 degrees:
## after rotation with 180 degrees now the light is at the left of the world(model).
![image](https://user-images.githubusercontent.com/75092336/111078772-d3cebb80-84ff-11eb-8ffa-eb469e51faf6.png)

## FLAT vs Gouruad vs Phong Shading.

![image](https://user-images.githubusercontent.com/75092336/111078814-07a9e100-8500-11eb-8618-a46bbb26b6ef.png)
![image](https://user-images.githubusercontent.com/75092336/111078821-0d072b80-8500-11eb-9c27-fb788ea1bc58.png)
![image](https://user-images.githubusercontent.com/75092336/111078840-21e3bf00-8500-11eb-9e87-655fab661154.png)

## shading code :
![image](https://user-images.githubusercontent.com/75092336/111078891-58b9d500-8500-11eb-911a-a786c84609e6.png)
![image](https://user-images.githubusercontent.com/75092336/111078877-4c357c80-8500-11eb-90bd-5fc8ac64c39e.png)
![image](https://user-images.githubusercontent.com/75092336/111078902-61121000-8500-11eb-98ed-e29a3b102304.png)

# in flat we calculate the light according to the face normal, in gouraud we calculate the light for each vertix 
# using its normal from assignment 1 where in Phong for each pixel we calculate its normal and then compute the lighting
# for the specific pixel.

## color specification
# ambient Red, Diffuse Green, Specular Blue.
![image](https://user-images.githubusercontent.com/75092336/111079079-12b14100-8501-11eb-808c-b648e62a5588.png)
# Material specification 
![image](https://user-images.githubusercontent.com/75092336/111079166-63c13500-8501-11eb-9ed0-6ce2bcfeda3a.png)
![image](https://user-images.githubusercontent.com/75092336/111079172-6e7bca00-8501-11eb-9943-e7e92655aa08.png)

# 5. BLUR:
![image](https://user-images.githubusercontent.com/75092336/111079306-201afb00-8502-11eb-92e8-9256a1462c10.png)
![image](https://user-images.githubusercontent.com/75092336/111079188-8bb09880-8501-11eb-8bb0-a8227741f321.png)
## code for the blur : 
![image](https://user-images.githubusercontent.com/75092336/111079231-bef32780-8501-11eb-956a-4e4efca1a0bd.png)
## the idea was to go through the buffer and calculate the mean of each pixel with its neighbouring pixels
## 3x3 square.
