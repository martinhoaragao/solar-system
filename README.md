# Phase 1 - Graphical Primitives

This phase requires two applications: one to generate files with the models information
(in this phase only generate the vertices for the model) and the engine itself which will
read a configuration file, written in XML, and display the models.

To create the model files an application (independent from the engine) will receive as
parameters the graphical primitive’s type, other parameters required for the model creation,
and the destination file where the vertices will be stored.

In this phase the following graphical primitives are required:
- Plane (a square in the XZ plane, centred in the origin, made with 2 triangles)
- Box (requires X, Y and Z dimensions)
- Sphere (requires radius, slices and stacks)
- Cone (requires bottom radius, height, slices and stacks)

For instance, if we wanted to create a sphere with radius 1, 10 slices, 10 stacks, and store the
resulting vertices in file sphere.3d, and assuming our application is called generator, we could
write:

    generator sphere 1 10 10 shpere.3d

The file format should be defined by the students and can support additional information
to assist the reading process, for example, the file may contain a line at the beginning
stating the number of vertices it contains.

Afterwards, the engine will receive a configuration file, written in XML. In this phase the XML
file will contain only the indication of which previously generated files to load.

Several alternatives are available to assist the XML Reading, such as tinyXML
(http://www.grinninglizard.com/tinyxml/).

Example of a XML configuration file for phase one:

    <scene>
      <model file="sphere.3d" />
      <model file="plane.3d" />
    </scene>

Note: in the above example it is assumed that the files “sphere.3d” and “plane.3d” have been
previously created with the generator application.
The demo scenes for this phase are the XML configuration and model files to display
each graphical primitive.
