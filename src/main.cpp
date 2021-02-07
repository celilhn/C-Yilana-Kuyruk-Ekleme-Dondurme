
/// b161210082   Recep Onur ŞIKER
// b161210058 Cellihan KADIOĞLU


#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include<glad/glad.h>
#include<glm/vec3.hpp>    
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
#include<vector>
#include<cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "shaderprogram.hpp"
#include "square.hpp"
#include "TextureManager.hpp"
float length=0.2f;
float camUzaklik=1.0f;
float dondurmeYonu=0.0f;
struct Vertex
{
    glm::vec3   pos;
    glm::vec2   tex;
};

std::vector<Vertex>         vertices;
std::vector<unsigned int>   indices;
std::vector<Square*>    snakeList;
glm::mat4   matTransform;

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;   //index buffer'ın id değerini tutacak

unsigned int Textures[6];
int textureSayac=0;

Vertex Dizi[24];
Vertex Kaplama[24];



void moveSnake()
{
    for(auto next:snakeList)
    {
        next->move();

    }

    for(int i=snakeList.size()-1;i>0;i--)
    {
        snakeList[i]->setDirection(snakeList[i-1]->getDirection());
    }


        
}
void drawSnake(ShaderProgram& program)
{

    for(auto next:snakeList)
    {
        glBindTexture(GL_TEXTURE_2D, next->getTexture());
                glActiveTexture(GL_TEXTURE0);
         
        program.setVec3("uMove",next->getPosition());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
 
    }


}
void addToSnakeTail()
{
    int elementCount = snakeList.size();


    if(elementCount==0)
    {
        snakeList.push_back(new Square(0.0f,0.0f,length));
    }
    else
    {
        Square* lastSquare = snakeList[elementCount-1];

        glm::vec3 pos = lastSquare->getPosition();

        Square::DIRECTION dir = lastSquare->getDirection();

        switch(dir)
        {
            case Square::DIR_RIGHT:
                pos-=glm::vec3(length,0.0f,0.0f);
            break;
            case  Square::DIR_LEFT:
                pos+=glm::vec3(length,0.0f,0.0f);
                break;
            case  Square::DIR_UP:
                pos-=glm::vec3(0.0f,length,0.0f);
                break;    
            case  Square::DIR_DOWN:
                pos+=glm::vec3(0.0f,length,0.0f);
                break;              
        }
        
        Square* newSquare = new Square(pos.x,pos.y,length);
        if(textureSayac==6)
        {
            textureSayac=0;
        }
        else
        {
            textureSayac++;
        }
        
        newSquare->setTexture(Textures[textureSayac]);
        newSquare->setDirection(dir);

        snakeList.push_back(newSquare);
    


    }
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();   
    if(action==GLFW_PRESS)
    {
        if(snakeList.size()!=0)
        {
            Square* first = snakeList[0];

            if(key==GLFW_KEY_LEFT)  
            {
                first->setDirection(Square::DIR_LEFT);
            }
            if(key==GLFW_KEY_RIGHT) 
            {
                first->setDirection(Square::DIR_RIGHT);
            }    
            if(key==GLFW_KEY_UP)  
            {
                first->setDirection(Square::DIR_UP);
            }
            if(key==GLFW_KEY_DOWN) 
            {
                first->setDirection(Square::DIR_DOWN);
            }                
        }
        
        if(key==GLFW_KEY_SPACE)
        {
            addToSnakeTail();
        }

        if(key==GLFW_KEY_A)
        {
            camUzaklik+=0.3f;
            
           


        }
        if(key==GLFW_KEY_D)
        {   
            if(camUzaklik > 1.3f)
           { camUzaklik-=0.3f;}
            
            
           

        }
           
    }  
           
}

int main(int argc,char** argv)
{   
    std:: srand(time(0));
    if(std:: rand()%10<5)
    {
        dondurmeYonu=4.0f;
    }
    else
    {
        dondurmeYonu=-4.0f;
    }
    
     /*       float radius=0.1f; int vertexCount=24;
     float angle = 360.0f/vertexCount;

    int triangleCount = vertexCount-2;
    
    
    // std::vector<glm::vec3>   tempVertices;
 
    for(int i=0;i<vertexCount;i++)
    {
        float newAngle = angle*i;  

        float x= radius*cos(glm::radians(newAngle));

        float y= radius*sin(glm::radians(newAngle));
        
        float z = 0.0f;
        Dizi[i].pos=glm::vec3(x,y,z); 
        Dizi[i].tex=glm::vec2(x,y);
       // tempVertices.push_back(glm::vec3(x,y,z));
       vertices.push_back(Dizi[i]);

    }


    for(int i=0;i<triangleCount;i++)
    {
      
    
        
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
        
    }     ****************************************/
    Dizi[0].pos=glm::vec3 ( length/2,length/2,length/2);
    Dizi[1].pos=glm::vec3 ( -length/2,+length/2,length/2);
    Dizi[2].pos=glm::vec3 ( -length/2,-length/2,length/2);
    Dizi[3].pos=glm::vec3 ( +length/2,-length/2,length/2);
    Dizi[4].pos=glm::vec3 ( -length/2,-length/2,-length/2);
    Dizi[5].pos=glm::vec3 ( +length/2,-length/2,-length/2);
    Dizi[6].pos=glm::vec3 ( -length/2,+length/2,-length/2);
    Dizi[7].pos=glm::vec3 ( +length/2,+length/2,-length/2);

    Kaplama[0].pos=Dizi[1].pos;
    Kaplama[1].pos=Dizi[2].pos;
    Kaplama[2].pos=Dizi[3].pos;
    Kaplama[3].pos=Dizi[0].pos;
    Kaplama[0].tex=glm::vec2(0.0f,1.0f);
    Kaplama[1].tex=glm::vec2(0.0f,0.0f);
    Kaplama[2].tex=glm::vec2(1.0f,0.0f);
    Kaplama[3].tex=glm::vec2(1.0f,1.0f);

                                    //ön yüz

    Kaplama[4].pos=Dizi[0].pos;
    Kaplama[5].pos=Dizi[3].pos;
    Kaplama[6].pos=Dizi[5].pos;
    Kaplama[7].pos=Dizi[7].pos;
    Kaplama[4].tex=glm::vec2(0.0f,1.0f);
    Kaplama[5].tex=glm::vec2(0.0f,0.0f);
    Kaplama[6].tex=glm::vec2(1.0f,0.0f);
    Kaplama[7].tex=glm::vec2(1.0f,1.0f);
                                    // sağ yüz

    Kaplama[8].pos=Dizi[6].pos;
    Kaplama[9].pos=Dizi[1].pos;
    Kaplama[10].pos=Dizi[0].pos;
    Kaplama[11].pos=Dizi[7].pos;
    Kaplama[8].tex=glm::vec2(0.0f,1.0f);
    Kaplama[9].tex=glm::vec2(0.0f,0.0f);
    Kaplama[10].tex=glm::vec2(1.0f,0.0f);
    Kaplama[11].tex=glm::vec2(1.0f,1.0f);
                                    //üst yüz

    Kaplama[12].pos=Dizi[4].pos;
    Kaplama[13].pos=Dizi[2].pos;
    Kaplama[14].pos=Dizi[3].pos;
    Kaplama[15].pos=Dizi[5].pos;
    Kaplama[12].tex=glm::vec2(0.0f,1.0f);
    Kaplama[13].tex=glm::vec2(0.0f,0.0f);
    Kaplama[14].tex=glm::vec2(1.0f,0.0f);
    Kaplama[15].tex=glm::vec2(1.0f,1.0f); 
                                   //alt yüz
    Kaplama[16].pos=Dizi[6].pos;
    Kaplama[17].pos=Dizi[4].pos;
    Kaplama[18].pos=Dizi[5].pos;
    Kaplama[19].pos=Dizi[7].pos;
    Kaplama[16].tex=glm::vec2(0.0f,1.0f);
    Kaplama[17].tex=glm::vec2(0.0f,0.0f);
    Kaplama[18].tex=glm::vec2(1.0f,0.0f);
    Kaplama[19].tex=glm::vec2(1.0f,1.0f); 
                                  //arka yüz
    Kaplama[20].pos=Dizi[1].pos;
    Kaplama[21].pos=Dizi[2].pos;
    Kaplama[22].pos=Dizi[4].pos;
    Kaplama[23].pos=Dizi[6].pos;
    Kaplama[20].tex=glm::vec2(0.0f,1.0f);
    Kaplama[21].tex=glm::vec2(0.0f,0.0f);
    Kaplama[22].tex=glm::vec2(1.0f,0.0f);
    Kaplama[23].tex=glm::vec2(1.0f,1.0f);                              
                                    //sol yüz

   /* vertices.push_back(Dizi[0]);
    vertices.push_back(Dizi[1]);
    vertices.push_back(Dizi[2]);
    vertices.push_back(Dizi[3]);
    vertices.push_back(Dizi[4]);
    vertices.push_back(Dizi[5]);
    vertices.push_back(Dizi[6]);
    vertices.push_back(Dizi[7]);*/
    for (int i=0;i<24;i++)
    {
        vertices.push_back(Kaplama[i]);
    }
    for (int i=0;i<6;i++)
    {
        int startindex=4*i;
        indices.push_back(startindex);
        indices.push_back(startindex+1); 
        indices.push_back(startindex+2);

        indices.push_back(startindex);
        indices.push_back(startindex+2); 
        indices.push_back(startindex+3);
    }


    /*    indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(0);
                            // ön 
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(5);

        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(7);
                            //sağ
        indices.push_back(4);
        indices.push_back(2);
        indices.push_back(3);

        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(5);
                            //alt
        indices.push_back(6);
        indices.push_back(1);
        indices.push_back(0);

        indices.push_back(6);
        indices.push_back(0);
        indices.push_back(7);
                            //üst
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(5);

        indices.push_back(6);
        indices.push_back(5);
        indices.push_back(7);
                            //arka
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(4);

        indices.push_back(1);
        indices.push_back(4);
        indices.push_back(6);
                            //sol
                            */
           
 
    if(!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800,800,"İLk Programım",NULL,NULL);

    if(window==NULL)
    {
        std::cout<<"Pencere Olusturulamadi"<<std::endl;

        glfwTerminate();

        return -1;
    }    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 
   

    TextureManager* textureManager = TextureManager::getInstance();

    unsigned int textureId = textureManager->loadTexture("./images/container.jpg");
    unsigned int textureId1 = textureManager->loadTexture("./images/homer.png");
    unsigned int textureId2 = textureManager->loadTexture("./images/saat.jpg");
    unsigned int textureId3 = textureManager->loadTexture("./images/lisa.png");
    unsigned int textureId4 = textureManager->loadTexture("./images/pusula.jpg");
    unsigned int textureId5 = textureManager->loadTexture("./images/brick.jpg");
   Textures[0]=textureId1;
   Textures[1]=textureId;
   Textures[2]=textureId2;
   Textures[3]=textureId3;
   Textures[4]=textureId4;
   Textures[5]=textureId1;

    ShaderProgram program;

    program.attachShader("./shaders/simplevs.glsl",GL_VERTEX_SHADER);
    program.attachShader("./shaders/simplefs.glsl",GL_FRAGMENT_SHADER);
    program.link();

    program.addUniform("uMove");
    program.addUniform("uColor");
    program.addUniform("uTransform");
    program.addUniform("uMtxTransform");

    glm::mat3 mtxTransform(1);
    mtxTransform = glm::rotate(glm::mat3(1),glm::radians(15.0f));

    matTransform = glm::mat4(1.0f);

    matTransform = glm::rotate(matTransform,glm::radians(5.0f), glm::vec3(0.0f,0.0f,1.0f));

    glGenVertexArrays(1, &VAO); 

    glGenBuffers(1,&VBO);

    glGenBuffers(1, &EBO);
   
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //dairenin vertex bilgileri vertex buffer a koplayanıyor
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
    
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
     
    glEnableVertexAttribArray(0); 
   
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3 * sizeof(float)));
       
    glEnableVertexAttribArray(1);   
    
    //daireye ait index değerleri index buffer'a kopyalanıyor.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW); 
   
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    
        glm::mat4 mtxProj =glm:: perspective(glm::radians(90.0f),(800.0f/800.0f),1.0f,100.0f);

        glm::vec3 camPosition(0.0f,0.3f,camUzaklik);
        glm::vec3 camLookAt(0.0f,0.0f,0.0f);          
        glm :: vec3 camUp(0.0f,1.0f,0.0f);
        glm::mat4 mtxCam = glm ::lookAt(camPosition,camLookAt,camUp);
        float z=1.0f;
    glEnable(GL_DEPTH_TEST);

    float aci = 1.0f;
    while(!glfwWindowShouldClose(window))
    {
        //oluşturulacak resim başlangıç rengine boyanıyor
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
       
        glm::vec3 camPosition(0.0f,0.3f,camUzaklik);
    
        mtxCam= glm ::lookAt(camPosition,camLookAt,camUp);
        //çizimde kullanılacak olan program nesnesi aktif ediliyor
        program.use();
        
       // glActiveTexture(GL_TEXTURE0);
       //  glBindTexture(GL_TEXTURE_2D, textureId);
        
        //çizimde kullanılacak olan Vertex array object aktif ediliyor
        glBindVertexArray(VAO);
        //çizim komutu gönderiliyor
         ///1.Kare
       // program.setVec3("uMove",glm::vec3(0.0f,0.0f,0.0f));
      //  program.setVec4("uColor",glm::vec4(1.0f,0.0f,0.0f,1.0f));
        //   program.setMat4("uTransform",&matTransform);
        
        mtxTransform = glm::rotate(glm::mat3(1),glm::radians(aci));
     //   mtxToplam= mtxProj*mtxCam*mtx
       //  program.setMat3("uMtxTransform",&mtxTransform);
        //daire index buffer kullanılarak kopyalanıyor.
       // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        drawSnake(program);

        moveSnake(); 

        std::this_thread::sleep_for (std::chrono::milliseconds(70));
    
        glm::mat4 matRotation   = glm::rotate(glm::mat4(1),glm::radians(aci), glm::vec3(0.0f,0.0f,1.0f));
        //glm::mat4 matTranslate  = glm::translate(glm::mat4(1),glm::vec3(0.3,0.3,0.3));
 
        matTransform = mtxProj*mtxCam;
        program.setMat4("uTransform",&matTransform);
         program.setMat3("uMtxTransform",&mtxTransform);
        aci+=dondurmeYonu;
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

}