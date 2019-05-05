#include "../include/Model.h"
#include <sstream>
#include <math.h>
using namespace std;
#define NO_TRG_CIRCLE 10

Model::Model(){
    m_translate = glm::mat4(1.0f);
    m_rotate = glm::mat4(1.0f);
    m_scale = glm::mat4(1.0f);
    m_revolve = glm::mat4(1.0f);
    is_selected = false;
    is_light_source = false;
    is_select_rotate = false;
    no_m_press = 0;
    no_t_press = 0;
    angle = 0;
    rev_angle = 0;
    period = 0;
    motion = 0;
}

Model::~Model(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

string Model::get_file_format() const{
    return file_format;
}

string Model::get_data_format() const{
    return data_format;
}

vector<string> Model::get_comments() const{
    return comments;
}

GLfloat Model::get_version_no() const{
    return version_no;
}

int Model::get_no_vertices() const{
    return no_vertices;
}

int Model::get_no_faces() const{
    return no_faces;
}

glm::mat4 Model::get_translate() const{
    return m_translate;
}

glm::mat4 Model::get_scale() const{
    return m_scale;
}

glm::vec3 Model::get_lightpos() const{
    return lightpos;
}

glm::mat4 Model::get_rotate() const{
    return m_rotate;
}

vector<GLfloat> Model::get_vertices() const {
    return vertices;
}

vector<unsigned int> Model::get_indices() const {
    return indices;
}

vector<Color> Model::get_vertices_color() const{
    return vertices_color;
}

glm::vec3 Model::get_cursor_pos() const {
    return currpos;
}

glm::mat4 Model::get_model() const{
    return m_translate*m_rotate*m_scale;
}

unsigned int Model::get_VAO() const{
    return VAO;
}

const Texture & Model::get_texture() const{
    return texture;
}

float Model::get_angle() const {
    return angle;
}

bool Model::get_light_source() const {
    return (is_light_source == true);
}

bool Model::is_select() const{
    return is_selected;
}

bool Model::get_is_select_rotate() const{
    return (is_select_rotate == true);
}

Color Model::get_vertex_color(int idx) const{
    return vertices_color[idx];
}

vector<float> Model::get_spherical() const {
    return spherical;
}

vector<float> Model::get_cylindrical() const {
    return cylindrical;
}

vector<float> Model::get_plane() const {
    return plane;
}

int Model::get_id() const {
    return id;
}

Point Model::get_mini() const {
    return mini;
}

Point Model::get_maxi() const {
    return maxi;
}

glm::mat4 Model::get_revolve() const {
    return m_revolve;
}

float Model::get_rev_angle() const {
    return rev_angle;
}

glm::vec3 Model::get_rev_axis() const {
    return rev_axis;
}

glm::vec3 Model::get_center() const {
    return glm::vec3(get_model() * glm::vec4(0.0, 0.0, 0.0, 1.0));
}

void Model::set_vertex_color(int idx,const Color& color){
    vertices_color[idx] = color;
}

void Model::set_cursor_pos(const glm::vec3& pos){
    currpos = pos;
}

void Model::set_translate(const glm::mat4& tr){
    m_translate = tr;
}

void Model::set_light_source(bool val) {
    is_light_source = val;
}

void Model::set_scale(const glm::mat4& sc) {
    m_scale = sc;
}

void Model::set_rev_axis(glm::vec3 axis) {
    rev_axis = axis;
}

void Model::set_lightpos(glm::vec3 pos) {
    lightpos = pos;
}

void Model::set_rev_angle(float angle) {
    rev_angle = angle;
}

void Model::set_rotate(const glm::mat4& rr) {
    m_rotate = rr;
}

void Model::set_selected(bool val) {
    is_selected = val;
}

void Model::set_light_no(int no) {
    light_no = no;
}

void Model::set_select_rotate(bool val) {
    is_select_rotate = val;
}

void Model::set_revolve(const glm::mat4& rv) {
    m_revolve = rv;
}

void Model::set_texture(const Texture& tex) {
    texture = tex;
}

void Model::set_id(int num) {
    id = num;
}

void Model::set_angle(float ang){
    angle = ang;
}

void Model::set_motion(int m) {
    motion = m;
}

void Model::translate(glm::vec3 tr) {
    m_translate = glm::translate(m_translate,tr);
}

void Model::rotate(float angle,glm::vec3 axis) {
    m_rotate = glm::rotate(m_rotate,angle,axis);
}

void Model::scale(glm::vec3 sc) {
    m_scale = glm::scale(m_scale,sc);
}

void Model::change_mapping() {
    glBindBuffer(GL_ARRAY_BUFFER,textureVBO);
    if (no_m_press == 0) {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*plane.size(),
                            &plane[0],GL_STATIC_DRAW);
    } else if (no_m_press == 1) {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*spherical.size(),
                            &spherical[0],GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*cylindrical.size(),
                            &cylindrical[0],GL_STATIC_DRAW);
    }
}

void Model::pass_info_lightshader(){
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);
    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices()[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*get_indices().size(),
                                            &get_indices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
}

void Model::pass_info_shader(){
    if(is_light_source){
        pass_info_lightshader();
        return;
    }
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);
    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices()[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*get_indices().size(),
                                            &get_indices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    
    unsigned int ColorBuffer;
    glGenBuffers(1,&ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices_color()[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER,textureVBO);
    if (no_m_press == 0) {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*plane.size(),
                            &plane[0],GL_STATIC_DRAW);
    } else if (no_m_press == 1) {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*spherical.size(),
                            &spherical[0],GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*cylindrical.size(),
                            &cylindrical[0],GL_STATIC_DRAW);
    }
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
}

bool is_between(float mini,float maxi,float value){
    return (value>=mini && value<=maxi);
}

bool Model::is_inside(Point trans_coord){
    // cout << "mini:" << mini << endl;
    // cout << "maxi:" << maxi << endl;
    if(is_between(mini.getX(),maxi.getX(),trans_coord.getX()) &&
      is_between(mini.getY(),maxi.getY(),trans_coord.getY()) && 
      is_between(mini.getZ(),maxi.getZ(),trans_coord.getZ())){
        return true;
    }
    else{
        return false;
    }
}

void Model::set_mini(const Point& point){
    mini = point;
}

void Model::set_maxi(const Point& point){
    maxi = point;
}

void Model::set_spherical(const vector<float>& sph) {
    spherical = sph;
}

void Model::set_cylindrical(const vector<float>& cyl) {
    cylindrical = cyl;
}

void Model::set_plane(const vector<float>& pl) {
    plane = pl;
}

void Model::compute_adj_list(){
    int no_trg = indices.size()/3;
    adj_list.resize(vertices.size()/3);
    for(int i = 0; i < no_trg; i++){
        adj_list[indices[i*3]].push_back(i);
        adj_list[indices[i*3+1]].push_back(i);
        adj_list[indices[i*3+2]].push_back(i);
    }
}

vector<Point> Model::compute_normals() {
    int no_trg = indices.size()/3;
    vector<Point> trg_normal;
    for(int i = 0; i < no_trg; i++){
        Point p1 = Point(vertices[3*indices[i*3+0]],vertices[3*indices[i*3+0]+1],vertices[3*indices[i*3+0]+2]);
        Point p2 = Point(vertices[3*indices[i*3+1]],vertices[3*indices[i*3+1]+1],vertices[3*indices[i*3+1]+2]);
        Point p3 = Point(vertices[3*indices[i*3+2]],vertices[3*indices[i*3+2]+1],vertices[3*indices[i*3+2]+2]);
        // cout << i << p1 << p2 << p3;
        Point V = p2-p1;
        Point W = p3-p1;
        Point normal;
        normal.setX(V.getY()*W.getZ() - V.getZ()*W.getY());
        normal.setY(V.getZ()*W.getX() - V.getX()*W.getZ());
        normal.setZ(V.getX()*W.getY() - V.getY()*W.getX());
        float mag = sqrt(normal.getX()*normal.getX() + normal.getY()*normal.getY() + normal.getZ()*normal.getZ());
        normal.normalize(mag); 
        // cout << i << " "<< normal;
        trg_normal.push_back(normal);
    }
    return trg_normal;
}

void Model::compute_vertices_color(){
    vector<Point> trg_normal = compute_normals();
    compute_adj_list();
    for(int i = 0; i < adj_list.size(); i++){
        Point p(0,0,0);
        for(auto e:adj_list[i]){
            p = p + trg_normal[e];
        }
        p.normalize(adj_list[i].size());
        float mag = sqrt( p.getX()*p.getX() + p.getY()*p.getY() + p.getZ()*p.getZ() );
        p.normalize(mag);
        vertices_color.push_back(Color(abs(p.getX()),abs(p.getY()),abs(p.getZ())));
    }   
}

void Model::compute_spherical() {
    glm::vec3 center;
    center.x = (maxi.getX() + mini.getX()) * 0.5f;
    center.y = (maxi.getY() + mini.getY()) * 0.5f;
    center.z = (maxi.getZ() + mini.getZ()) * 0.5f;
    float r = (maxi.getX() - mini.getX()) * 0.5f;
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec3 p;
        p.x = vertices[i*3];
        p.y = vertices[i*3+1];
        p.z = vertices[i*3+2];
        glm::vec2 tex;
        p.x = p.x - center.x;
        p.y = p.y - center.y;
        p.z = p.z - center.z;
        tex.x = atan2(-p.z, p.x) / (2 * 3.14);
        tex.x += 3.14;
        tex.y = (acos(-p.y / r) / 3.14);
        spherical.push_back(tex.x);
        spherical.push_back(tex.y);
    }
}

void Model::compute_cylindrical() {
    float height = maxi.getZ() - mini.getZ();
    float r = (maxi.getX() - mini.getX()) * 0.5f;
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec2 tex;
        glm::vec3 pos;
        pos.x = vertices[i*3];
        pos.y = vertices[i*3+1];
        pos.z = vertices[i*3+2];
        tex.x = acosf(pos.x / r) / (2 * 3.14f);
        tex.y = pos.z;
        cylindrical.push_back(tex.x);
        cylindrical.push_back(tex.y);
    }
}

void Model::compute_plane() {
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec2 tex;
        glm::vec3 pos;
        pos.x = vertices[i*3];
        pos.y = vertices[i*3+1];
        pos.z = vertices[i*3+2];
        tex.x = (pos.x - mini.getX()) / (maxi.getX() - mini.getX());
        tex.y = (pos.y - mini.getY()) / (maxi.getY() - mini.getY());
        plane.push_back(tex.x);
        plane.push_back(tex.y);
    }
}

void Model::compute_texture_mapping() {
    compute_spherical();
    compute_cylindrical();
    compute_plane();
}

void Model::addChild(Model * m) {
    children.push_back(m);
}

Point Model::transform(const Point& point,const Point& normal,const Point& incentre,float inradii){
    //do m_rotate first then translate and then scale.
    glm::vec3 circle_normal = glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 trg_normal = glm::vec3(normal.getX(),normal.getY(),normal.getZ());
    trg_normal = glm::normalize(trg_normal);
    glm::vec3 axis = glm::cross(trg_normal,circle_normal);
    axis = glm::normalize(axis);
    float angle = acos(glm::dot(circle_normal,trg_normal));

    glm::vec3 inctr = glm::vec3(incentre.getX(),incentre.getY(),incentre.getZ()); 
    glm::mat4 rotate = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);

    if(angle != 0)
        rotate = glm::rotate(rotate,-angle,axis);
    translate = glm::translate(translate,inctr);
    scale = glm::scale(scale,glm::vec3(inradii,inradii,inradii));

    glm::vec4 pt = glm::vec4(point.getX(),point.getY(),point.getZ(),1.0f);
    glm::mat4 main_matrix = translate*rotate*scale;
    glm::vec4 newpt = main_matrix*pt;

    return Point(newpt.x,newpt.y,newpt.z);
}

std::ifstream& safeGetline(std::ifstream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::ifstream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

//assumptions:
//vertices will be in float format only.
//no of dimensions = 3.
ifstream & operator >> (ifstream &fin, Model &model){
    string temp;
    fin >> temp >> model.file_format >> model.data_format;
    float minx=1e5,miny=1e5,minz=1e5;
    float maxx=-1e5,maxy=-1e5,maxz=-1e5;
    while(1){
        fin >> temp;
        if(temp != "comment") break;
        string comment;
        safeGetline(fin,comment);
        model.comments.push_back(comment);
    }
    fin >> temp >> model.no_vertices;
    while(1){
        fin >> temp;
        if(temp != "property") break;
        fin >> temp >> temp;
        // do nothing.
    }
    fin >> temp >> model.no_faces;
    while(1){
        safeGetline(fin,temp);
        if(temp == "end_header"){
            break;
        }
    }
    float maxi=0;
    for(int i = 0; i < model.no_vertices; i++){
        float temp1;
        float arr[3];
        //change to absolute value
        for(int j = 0; j < 3; j++){
            fin >> temp1;
            maxi=max(maxi,abs(temp1));
            model.vertices.push_back(temp1);
            if(j==0){
                minx = min(minx,temp1);
                maxx = max(maxx,temp1);
            }
            else if(j==1){
                miny = min(miny,temp1);
                maxy = max(maxy,temp1);
            }
            else{
                minz = min(minz,temp1);
                maxz = max(maxz,temp1);
            }
        }
    }
    float maxdim = max(maxx - minx, max(maxy - miny, maxz - minz));  
    for(int i=0;i<model.vertices.size()/3;i++){
        model.vertices[i*3] = (model.vertices[i*3] - minx )/ maxdim;
        model.vertices[i*3+1] = (model.vertices[i*3+1] - miny )/ maxdim;
        model.vertices[i*3+2] = (model.vertices[i*3+2] - minz )/ maxdim;
        // cout << "dart::" << model.vertices[i*3] << " " << model.vertices[i*3+1] << " "
        //  << model.vertices[i*3+2] << " " << endl;
    }
    for(int i = 0; i < model.no_faces; i++){
        unsigned int tmp;
        fin >> tmp;
        unsigned int tmp1;
        for(int j = 0; j < tmp; j++){
            fin >> tmp1;
            model.indices.push_back(tmp1);
        }
    }

    float minX = 2.0,minY=2.0,minZ=2.0;
    float maxX=-2.0,maxY=-2.0,maxZ=-2.0;
    for(int i=0;i<model.vertices.size()/3;i++){
        minX = min(minX,model.vertices[i*3]);
        maxX = max(maxX,model.vertices[i*3]);
        minY = min(minY,model.vertices[i*3+1]);
        maxY = max(maxY,model.vertices[i*3+1]);
        minZ = min(minZ,model.vertices[i*3+2]);
        maxZ = max(maxZ,model.vertices[i*3+2]);
    }

    model.set_mini(Point(minX,minY,minZ));
    model.set_maxi(Point(maxX,maxY,maxZ));

    //compute color of the vertices.
    model.compute_vertices_color();
    model.compute_texture_mapping();
    return fin;
}

void Model::update(float speed, glm::vec3 parent_center, glm::mat4 worldMatrix){
    glm::mat4 model = m_revolve*m_translate* m_rotate * m_scale;
    
    glm::vec3 center = glm::vec3(model*glm::vec4(0.0, 0.0, 0.0, 1.0));
    if(motion == 1){
        m_translate = glm::translate(m_translate, glm::vec3(0.0, 0.025, 0.0));
        period++;
        if(period == 20 - int(speed*100.0f)){
            period = 0;
            motion = -1;
        }
    }
    else if(motion == -1){
        m_translate = glm::translate(m_translate, glm::vec3(0.0, -0.025, 0.0));
        period++;
        if(period == 20 - int(speed*100.0f)){
            period = 0;
            motion = 1;
        }
    }
    else if(motion == 2){
        m_revolve = glm::rotate(m_revolve, 0.03f, glm::vec3(0.0, 0.0, 1.0));
        m_translate = glm::translate(m_translate, glm::vec3(0.025, 0.0, 0.0));
        period++;
        if(period == 10 - int(speed*100.0f)){
            period = 0;
            motion = -2;
        }
    }
    else if(motion == -2){
        m_revolve = glm::rotate(m_revolve, 0.03f + speed, glm::vec3(0.0, 0.0, 1.0));

        m_translate = glm::translate(m_translate, glm::vec3(-0.025, 0.0, 0.0));
        period++;
        if(period == 10 - int(speed*100.0f)){
            period = 0;
            motion = 2;
        }
    }
    else if(motion == 3){
        glm::vec3 towards = glm::vec3(glm::vec4(parent_center - center, 1.0f));
        towards = glm::normalize(towards) / (50.0f - 100.0f*speed);
        m_translate = glm::translate(m_translate, towards);
    }

    if(is_select_rotate)
    {
        m_rotate = glm::rotate(m_rotate, 0.05f + speed, glm::vec3(0.0, 1.0, 0.0));
    }

    for(int i = 0; i<children.size(); i++){
        children[i]->update(speed, center ,worldMatrix* m_translate *m_scale);
    }
}

void Model::setMotion(int m){
    motion = m;
}
