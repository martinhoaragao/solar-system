# Phase 2 – Geometric Transforms

This phase is about creating hierarchical scenes using geometric transforms.
Consider a scene defined as a tree where each node contains a set of geometric transforms
(translate, rotate and scale) and a set of models. Each node can also have children nodes.
Example of a configuration XML file with a single group:

  <scene>
    <group>
      <translate X=5 Y=0 Z=2 />
      <rotate angle=45 axisX=0 axisY=1 axisZ=0 />
      <models>
        <model file=”sphere.3d” />
      </models>
    </group>
  </scene>

Example of a group with a child group:

  <scene>
    <group>
      <translate X=1 />
      <models>
        <model file=”sphere.3d” />
      </models>
      <group>
        <translate Y=1 />
        <models>
          <model file=”cone.3d” />
        </models>
      </group>
    </group>
  </scene>

In the second example the child group will inherit the geometric transforms from the parent.
Note: the order of the geometric transforms is relevant.
The required demo scene for this phase is a static model of the solar system, including the sun,
planets and moons defined in a hierarchy.
