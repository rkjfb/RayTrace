// Based heavily on the PLY test example included with the Stanford PLY library.
#include "pch.h"
#include "PlyLoader.h"
#include "ply.h"

using namespace ray;

typedef struct Vertex {
    float x, y, z;             /* the usual 3-space position of a vertex */
} Vertex;

PlyProperty vert_props[] = { /* list of property information for a vertex */
  {(char*)"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {(char*)"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {(char*)"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
};

typedef struct Face {
    /*unsigned char intensity; this user attaches intensity to faces */
    unsigned char nverts;    /* number of vertex indices in list */
    int* verts;              /* vertex index list */
} Face;

PlyProperty face_props[] = { /* list of property information for a vertex */
//  {(char*)"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {(char*)"vertex_index", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};

PlyLoader::PlyLoader(const std::string& filename) {
    int i, j, k;
    PlyFile* ply;
    int nelems;
    char** elist;
    int file_type;
    float version;
    int nprops;
    int num_elems;
    PlyProperty** plist;
    Vertex** vlist;
    Face** flist;
    char* elem_name;
    int num_comments;
    char** comments;
    int num_obj_info;
    char** obj_info;

    vlist = 0;

    /* open a PLY file for reading */
    ply = ply_open_for_reading((char*)filename.c_str(), &nelems, &elist, &file_type, &version);

    /* print what we found out about the file */
    printf("version %f\n", version);
    printf("type %d\n", file_type);

    /* go through each kind of element that we learned is in the file */
    /* and read them */

    for (i = 0; i < nelems; i++) {

        /* get the description of the first element */
        elem_name = elist[i];
        plist = ply_get_element_description(ply, elem_name, &num_elems, &nprops);

        /* print the name of the element, for debugging */
        printf("element %s %d\n", elem_name, num_elems);

        /* if we're on vertex elements, read them in */
        if (equal_strings((char*)"vertex", elem_name)) {

            /* create a vertex list to hold all the vertices */
            vlist = (Vertex**)malloc(sizeof(Vertex*) * num_elems);

            /* set up for getting vertex elements */

            ply_get_property(ply, elem_name, &vert_props[0]);
            ply_get_property(ply, elem_name, &vert_props[1]);
            ply_get_property(ply, elem_name, &vert_props[2]);

            /* grab all the vertex elements */
            for (j = 0; j < num_elems; j++) {

                /* grab and element from the file */
                vlist[j] = (Vertex*)malloc(sizeof(Vertex));
                ply_get_element(ply, (void*)vlist[j]);

                /* print out vertex x,y,z for debugging */
                // printf("vertex: %g %g %g\n", vlist[j]->x, vlist[j]->y, vlist[j]->z);
            }
        }

        /* if we're on face elements, read them in */
        if (equal_strings((char*)"face", elem_name)) {

            /* create a list to hold all the face elements */
            flist = (Face**)malloc(sizeof(Face*) * num_elems);

            /* set up for getting face elements */

            ply_get_property(ply, elem_name, &face_props[0]);
           // ply_get_property(ply, elem_name, &face_props[1]);


            /* grab all the face elements */
            for (j = 0; j < num_elems; j++) {

                /* grab and element from the file */
                flist[j] = (Face*)malloc(sizeof(Face));
                ply_get_element(ply, (void*)flist[j]);

                /* print out face info, for debugging */
                /*
                printf("face: %d, list = ", flist[j]->intensity);
                for (k = 0; k < flist[j]->nverts; k++)
                    printf("%d ", flist[j]->verts[k]);
                printf("\n");
                */
                // fan triangulation.
                int first = flist[j]->verts[0];
                int last = flist[j]->verts[1];
                for (k = 2; k < flist[j]->nverts; k++) {
                    int latest = flist[j]->verts[k];

                    Point3 p1(vlist[first]->x, vlist[first]->y, vlist[first]->z);
                    Point3 p2(vlist[last]->x, vlist[last]->y, vlist[last]->z);
                    Point3 p3(vlist[latest]->x, vlist[latest]->y, vlist[latest]->z);
                    auto s = std::make_unique<Triangle>(p1, p2, p3);
                    shape.push_back(std::move(s));
                    last = latest;
                }
            }
        }

        /* print out the properties we got, for debugging */
        for (j = 0; j < nprops; j++)
            printf("property %s\n", plist[j]->name);
    }

    /* grab and print out the comments in the file */
    comments = ply_get_comments(ply, &num_comments);
    for (i = 0; i < num_comments; i++)
        printf("comment = '%s'\n", comments[i]);

    /* grab and print out the object information */
    obj_info = ply_get_obj_info(ply, &num_obj_info);
    for (i = 0; i < num_obj_info; i++)
        printf("obj_info = '%s'\n", obj_info[i]);

    /* close the PLY file */
    ply_close(ply);
}
