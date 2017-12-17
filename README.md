# AK Engine

AK Engine purpose is to be used as an experimentation and learning tool as well as a project for a Game Design & Development degree that we are currently coursing. This project is part of the subject [Videogame Engines](http://www.upc.edu/estudispdf/guia_docent.php?codi=804246&lang=cat).

## Team Members:
### [Adrián Castillo](https://github.com/adriancl1)

### [Marc López](https://github.com/MarcLF)
 Worked on: 
* ImGui Integration & Brofiler.
* Geometry rendered with OpenGL.
* Various modules like Importer or Window.
* Resource Mesh.
* Saving&Loading various Components & Scene.
* Quadtree implementation.
* Time Manager.
* Web.

## Main Core Sub-systems:
* Scene serialization: AK Engine is able to save, and then if ever needed load, every game object inside the main scene using our own format .akS.
* Mouse picking: You can select any game object currently being displayed into the scene by left-clicking on them.
* Quadtree: Every game object is currently organized inside a Quadtree for a possible future Quadtree acceleration if ever required.
* Resource Manager: AK Engine feature its own Resource Manager, currently administering every extern resources adapting them to our own format for a faster lecture process and only loads them once in memory even when they are used multiple times in the same scene.
* Time Management: Once running the engine you can enter in play mode and swap between Play, Pause, Stop and Play One Frame states giving you control over as how time goes in your game.

### Skeletal Animation
* AK Engine main sub-system that features is Skeletal Animation. This allows the user to Load animations using .DAE or .FBX format and play them on a skeleton which later on can be attached to a deformable mesh that will modify its position according to the bones transformation. 

Below you will find several gifs showing several functions of AK Engine Skeletal animation sub-system:
![GIF1](https://user-images.githubusercontent.com/17148658/34083627-c5dfb492-e373-11e7-940e-0965ee869282.gif)

![GIF2](https://user-images.githubusercontent.com/17148658/34083655-2f5272c0-e374-11e7-8488-e186a5f18c7c.gif)

Blending is also an optional feature to be abled or disabled depending on the needs of the user. AK Engine Skeletal Animation also allows you to create as many animation clips as needed for each individual animation or you can also set a custom start/end point in each one of them.

## Controls
* To Collapse - Uncollapse windows use LMB.
* To interact with check boxes use LMB.
* To add or remove any kind of integer value use LMB on the respective button (press + if you want to add +1 or - if you want to add -1 * to the corresponding integer).
* To rotate the camera use right click.
* To move the camera use: A/D (left/right), W/S (forwards/backwards), R/F (up/down). To move faster hold down L-Shift.
* Drag and drop a .fbx or .dae file to import it into the scene.
* Drag and drop a .wav file to play it.
* Click into a game object to select it.
* Use configuration to activate quadtree acceleration, drawing of quadtree and debug features.
* While in Demo scene, press 1 to play Skeleton attack animation and 2 to play the Skeleton walk animation.

## Changelog
Version 0.5.5 (Assignment 2) 
* Added acceleration through quadtree for mouse picking & camera culling.
* Textures resources are now updated on runtime if the original file is changed.
* Camera centers and focuses around the selected gameobject.
* You can add and delete mesh and material components.
* Bug Fixes.

Version 0.5.0 
* Added Resource Management.
* Quadtree is now adaptative.
* Added ImGuizmos.
* Save&Load working with resources.
* Creation of empty gameobjects in runtime.
* Bug Fixes.

Version 0.4.0 
* Added Mouse Picking.
* Added Time Manager.
* Added Scene Save&Load.
* Bug Fixes.

Version 0.3.7
* Added Quadtree.
* Added Frustum Culling.
* Added Camera.
* Bug Fixes.

Version 0.3.5
* Now you can change the transformation of meshes/GameObjects.
* New camera component, with settings for position and FOV.
* Partially implemented camera culling (still buggy).

Version 0.3.1
* New editor window that displays loaded game objects with their transformation, meshes and materials.

Version 0.3
* Textures drawn on cube with direct mode, indices and meshes.

Version 0.2.8
* Cube draw through vertex array and draw elements (indices).
* Import .fbx scenes and play .wav files through drag and drop.
* New debug features on meshes.

Version 0.2.7
* Cube draw through Direct Mode.
* UI design updated.
* Various new options to create different geometries.
* Removed Glut usage.
* Brofiler now displaying more information.

Version 0.2.5:
- Added Scene with a grid.
- Added options to configuration window, including: Audio volume & pause/resume, creation of 3D geometry into the scene, and more hardware info.
- Camera movement 
- Most changes on configuration are saved when you exit the engine.

Version 0.2:
- Added Configuration window.
- Brofiler Integrated.
- Added Json & Parson.

Version 0.1:
- Added Console.
- MathGeoLib integration.
- ImGui integration.
- Base code added.

## Credits

Adrian Castillo - https://github.com/adriancl1 

Marc López - https://github.com/MarcLF

                                 Apache License
                           Version 2.0, January 2004
                        http://www.apache.org/licenses/

   TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

   1. Definitions.

      "License" shall mean the terms and conditions for use, reproduction,
      and distribution as defined by Sections 1 through 9 of this document.

      "Licensor" shall mean the copyright owner or entity authorized by
      the copyright owner that is granting the License.

      "Legal Entity" shall mean the union of the acting entity and all
      other entities that control, are controlled by, or are under common
      control with that entity. For the purposes of this definition,
      "control" means (i) the power, direct or indirect, to cause the
      direction or management of such entity, whether by contract or
      otherwise, or (ii) ownership of fifty percent (50%) or more of the
      outstanding shares, or (iii) beneficial ownership of such entity.

      "You" (or "Your") shall mean an individual or Legal Entity
      exercising permissions granted by this License.

      "Source" form shall mean the preferred form for making modifications,
      including but not limited to software source code, documentation
      source, and configuration files.

      "Object" form shall mean any form resulting from mechanical
      transformation or translation of a Source form, including but
      not limited to compiled object code, generated documentation,
      and conversions to other media types.

      "Work" shall mean the work of authorship, whether in Source or
      Object form, made available under the License, as indicated by a
      copyright notice that is included in or attached to the work
      (an example is provided in the Appendix below).

      "Derivative Works" shall mean any work, whether in Source or Object
      form, that is based on (or derived from) the Work and for which the
      editorial revisions, annotations, elaborations, or other modifications
      represent, as a whole, an original work of authorship. For the purposes
      of this License, Derivative Works shall not include works that remain
      separable from, or merely link (or bind by name) to the interfaces of,
      the Work and Derivative Works thereof.

      "Contribution" shall mean any work of authorship, including
      the original version of the Work and any modifications or additions
      to that Work or Derivative Works thereof, that is intentionally
      submitted to Licensor for inclusion in the Work by the copyright owner
      or by an individual or Legal Entity authorized to submit on behalf of
      the copyright owner. For the purposes of this definition, "submitted"
      means any form of electronic, verbal, or written communication sent
      to the Licensor or its representatives, including but not limited to
      communication on electronic mailing lists, source code control systems,
      and issue tracking systems that are managed by, or on behalf of, the
      Licensor for the purpose of discussing and improving the Work, but
      excluding communication that is conspicuously marked or otherwise
      designated in writing by the copyright owner as "Not a Contribution."

      "Contributor" shall mean Licensor and any individual or Legal Entity
      on behalf of whom a Contribution has been received by Licensor and
      subsequently incorporated within the Work.

   2. Grant of Copyright License. Subject to the terms and conditions of
      this License, each Contributor hereby grants to You a perpetual,
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable
      copyright license to reproduce, prepare Derivative Works of,
      publicly display, publicly perform, sublicense, and distribute the
      Work and such Derivative Works in Source or Object form.

   3. Grant of Patent License. Subject to the terms and conditions of
      this License, each Contributor hereby grants to You a perpetual,
      worldwide, non-exclusive, no-charge, royalty-free, irrevocable
      (except as stated in this section) patent license to make, have made,
      use, offer to sell, sell, import, and otherwise transfer the Work,
      where such license applies only to those patent claims licensable
      by such Contributor that are necessarily infringed by their
      Contribution(s) alone or by combination of their Contribution(s)
      with the Work to which such Contribution(s) was submitted. If You
      institute patent litigation against any entity (including a
      cross-claim or counterclaim in a lawsuit) alleging that the Work
      or a Contribution incorporated within the Work constitutes direct
      or contributory patent infringement, then any patent licenses
      granted to You under this License for that Work shall terminate
      as of the date such litigation is filed.

   4. Redistribution. You may reproduce and distribute copies of the
      Work or Derivative Works thereof in any medium, with or without
      modifications, and in Source or Object form, provided that You
      meet the following conditions:

      (a) You must give any other recipients of the Work or
          Derivative Works a copy of this License; and

      (b) You must cause any modified files to carry prominent notices
          stating that You changed the files; and

      (c) You must retain, in the Source form of any Derivative Works
          that You distribute, all copyright, patent, trademark, and
          attribution notices from the Source form of the Work,
          excluding those notices that do not pertain to any part of
          the Derivative Works; and

      (d) If the Work includes a "NOTICE" text file as part of its
          distribution, then any Derivative Works that You distribute must
          include a readable copy of the attribution notices contained
          within such NOTICE file, excluding those notices that do not
          pertain to any part of the Derivative Works, in at least one
          of the following places: within a NOTICE text file distributed
          as part of the Derivative Works; within the Source form or
          documentation, if provided along with the Derivative Works; or,
          within a display generated by the Derivative Works, if and
          wherever such third-party notices normally appear. The contents
          of the NOTICE file are for informational purposes only and
          do not modify the License. You may add Your own attribution
          notices within Derivative Works that You distribute, alongside
          or as an addendum to the NOTICE text from the Work, provided
          that such additional attribution notices cannot be construed
          as modifying the License.

      You may add Your own copyright statement to Your modifications and
      may provide additional or different license terms and conditions
      for use, reproduction, or distribution of Your modifications, or
      for any such Derivative Works as a whole, provided Your use,
      reproduction, and distribution of the Work otherwise complies with
      the conditions stated in this License.

   5. Submission of Contributions. Unless You explicitly state otherwise,
      any Contribution intentionally submitted for inclusion in the Work
      by You to the Licensor shall be under the terms and conditions of
      this License, without any additional terms or conditions.
      Notwithstanding the above, nothing herein shall supersede or modify
      the terms of any separate license agreement you may have executed
      with Licensor regarding such Contributions.

   6. Trademarks. This License does not grant permission to use the trade
      names, trademarks, service marks, or product names of the Licensor,
      except as required for reasonable and customary use in describing the
      origin of the Work and reproducing the content of the NOTICE file.

   7. Disclaimer of Warranty. Unless required by applicable law or
      agreed to in writing, Licensor provides the Work (and each
      Contributor provides its Contributions) on an "AS IS" BASIS,
      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
      implied, including, without limitation, any warranties or conditions
      of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A
      PARTICULAR PURPOSE. You are solely responsible for determining the
      appropriateness of using or redistributing the Work and assume any
      risks associated with Your exercise of permissions under this License.

   8. Limitation of Liability. In no event and under no legal theory,
      whether in tort (including negligence), contract, or otherwise,
      unless required by applicable law (such as deliberate and grossly
      negligent acts) or agreed to in writing, shall any Contributor be
      liable to You for damages, including any direct, indirect, special,
      incidental, or consequential damages of any character arising as a
      result of this License or out of the use or inability to use the
      Work (including but not limited to damages for loss of goodwill,
      work stoppage, computer failure or malfunction, or any and all
      other commercial damages or losses), even if such Contributor
      has been advised of the possibility of such damages.

   9. Accepting Warranty or Additional Liability. While redistributing
      the Work or Derivative Works thereof, You may choose to offer,
      and charge a fee for, acceptance of support, warranty, indemnity,
      or other liability obligations and/or rights consistent with this
      License. However, in accepting such obligations, You may act only
      on Your own behalf and on Your sole responsibility, not on behalf
      of any other Contributor, and only if You agree to indemnify,
      defend, and hold each Contributor harmless for any liability
      incurred by, or claims asserted against, such Contributor by reason
      of your accepting any such warranty or additional liability.

   END OF TERMS AND CONDITIONS

   APPENDIX: How to apply the Apache License to your work.

      To apply the Apache License to your work, attach the following
      boilerplate notice, with the fields enclosed by brackets "{}"
      replaced with your own identifying information. (Don't include
      the brackets!)  The text should be enclosed in the appropriate
      comment syntax for the file format. We also recommend that a
      file or class name and description of purpose be included on the
      same "printed page" as the copyright notice for easier
      identification within third-party archives.

   Copyright {2017} {Marc López Fàbregas, Adrián Castillo López, Marc Fabián i Vilà}

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
