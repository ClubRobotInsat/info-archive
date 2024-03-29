// AxisSceneNode.h, found here: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=21993

#ifndef ROOT_AXISSCENENODE_H
#define ROOT_AXISSCENENODE_H

#include <irrlicht/irrlicht.h>

using namespace irr;

class AxesSceneNode : public scene::ISceneNode {
	scene::SMeshBuffer ZMeshBuffer;
	scene::SMeshBuffer YMeshBuffer;
	scene::SMeshBuffer XMeshBuffer;

	core::aabbox3df BoundingBox;

	video::SColor ZColor;
	video::SColor YColor;
	video::SColor XColor;

public:
	AxesSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id) {
#ifdef _DEBUG
		setDebugName("AxesSceneNode");
#endif

		u16 u[36] = {
		    0, 2, 1, 0, 3, 2, 1, 5, 4, 1, 2, 5, 4, 6, 7, 4, 5, 6, 7, 3, 0, 7, 6, 3, 3, 5, 2, 3, 6, 5, 0, 1, 4, 0, 4, 7,
		};
		ZMeshBuffer.Indices.set_used(36);
		YMeshBuffer.Indices.set_used(36);
		XMeshBuffer.Indices.set_used(36);
		// Color Settings
		ZColor = video::SColor(255, 0, 0, 255);
		YColor = video::SColor(255, 0, 255, 0);
		XColor = video::SColor(255, 255, 0, 0);
		for(s32 i = 0; i < 36; ++i) {
			ZMeshBuffer.Indices[i] = u[i];
			YMeshBuffer.Indices[i] = u[i];
			XMeshBuffer.Indices[i] = u[i];
		}
		// Default Position, Rotation and Scale
		this->setPosition(core::vector3df(0, 0, 0));
		this->setRotation(core::vector3df(0, 0, 0));
		this->setScale(core::vector3df(1, 1, 1));
		// setAutomaticCulling(scene::EAC_OFF);
		// Axes Box Coordinates Settings
		setAxesCoordinates();
	}

	~AxesSceneNode() override = default;

	void OnRegisterSceneNode() override {
		if(IsVisible) {
			SceneManager->registerNodeForRendering(this);
		}
		ISceneNode::OnRegisterSceneNode();
	}

	void render() override {
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setMaterial(ZMeshBuffer.Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(&ZMeshBuffer);

		driver->setMaterial(YMeshBuffer.Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(&YMeshBuffer);

		driver->setMaterial(XMeshBuffer.Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(&XMeshBuffer);
	}

	const core::aabbox3d<f32>& getBoundingBox() const override {
		return BoundingBox;
	}

	void setAxesCoordinates() {
		ZMeshBuffer.Vertices.set_used(8);
		ZMeshBuffer.Material.Wireframe = false;
		ZMeshBuffer.Material.Lighting = false;
		ZMeshBuffer.BoundingBox.reset(0, 0, 0);
		ZMeshBuffer.Vertices[0] = video::S3DVertex(-0.25, -0.25, 0, -1, -1, -1, ZColor, 0, 1);
		ZMeshBuffer.Vertices[1] = video::S3DVertex(0.25, -0.25, 0, 1, -1, -1, ZColor, 1, 1);
		ZMeshBuffer.Vertices[2] = video::S3DVertex(0.25, 0.25, 0, 1, 1, -1, ZColor, 1, 0);
		ZMeshBuffer.Vertices[3] = video::S3DVertex(-0.25, 0.25, 0, -1, 1, -1, ZColor, 0, 0);
		ZMeshBuffer.Vertices[4] = video::S3DVertex(0.25, -0.25, 25, 1, -1, 1, ZColor, 0, 1);
		ZMeshBuffer.Vertices[5] = video::S3DVertex(0.25, 0.25, 25, 1, 1, 1, ZColor, 0, 0);
		ZMeshBuffer.Vertices[6] = video::S3DVertex(-0.25, 0.25, 25, -1, 1, 1, ZColor, 1, 0);
		ZMeshBuffer.Vertices[7] = video::S3DVertex(-0.25, -0.25, 25, -1, -1, 1, ZColor, 1, 1);

		YMeshBuffer.Vertices.set_used(8);
		YMeshBuffer.Material.Wireframe = false;
		YMeshBuffer.Material.Lighting = false;
		YMeshBuffer.BoundingBox.reset(0, 0, 0);
		YMeshBuffer.Vertices[0] = video::S3DVertex(-0.25, 0, 0.25, -1, -1, -1, YColor, 0, 1);
		YMeshBuffer.Vertices[1] = video::S3DVertex(0.25, 0, 0.25, 1, -1, -1, YColor, 1, 1);
		YMeshBuffer.Vertices[2] = video::S3DVertex(0.25, 0, -0.25, 1, 1, -1, YColor, 1, 0);
		YMeshBuffer.Vertices[3] = video::S3DVertex(-0.25, 0, -0.25, -1, 1, -1, YColor, 0, 0);
		YMeshBuffer.Vertices[4] = video::S3DVertex(0.25, 25, 0.25, 1, -1, 1, YColor, 0, 1);
		YMeshBuffer.Vertices[5] = video::S3DVertex(0.25, 25, -0.25, 1, 1, 1, YColor, 0, 0);
		YMeshBuffer.Vertices[6] = video::S3DVertex(-0.25, 25, -0.25, -1, 1, 1, YColor, 1, 0);
		YMeshBuffer.Vertices[7] = video::S3DVertex(-0.25, 25, 0.25, -1, -1, 1, YColor, 1, 1);

		XMeshBuffer.Vertices.set_used(8);
		XMeshBuffer.Material.Wireframe = false;
		XMeshBuffer.Material.Lighting = false;
		XMeshBuffer.BoundingBox.reset(0, 0, 0);
		XMeshBuffer.Vertices[0] = video::S3DVertex(0, -0.25, 0.25, -1, -1, -1, XColor, 0, 1);
		XMeshBuffer.Vertices[1] = video::S3DVertex(0, -0.25, -0.25, 1, -1, -1, XColor, 1, 1);
		XMeshBuffer.Vertices[2] = video::S3DVertex(0, 0.25, -0.25, 1, 1, -1, XColor, 1, 0);
		XMeshBuffer.Vertices[3] = video::S3DVertex(0, 0.25, 0.25, -1, 1, -1, XColor, 0, 0);
		XMeshBuffer.Vertices[4] = video::S3DVertex(25, -0.25, -0.25, 1, -1, 1, XColor, 0, 1);
		XMeshBuffer.Vertices[5] = video::S3DVertex(25, 0.25, -0.25, 1, 1, 1, XColor, 0, 0);
		XMeshBuffer.Vertices[6] = video::S3DVertex(25, 0.25, 0.25, -1, 1, 1, XColor, 1, 0);
		XMeshBuffer.Vertices[7] = video::S3DVertex(25, -0.25, 0.25, -1, -1, 1, XColor, 1, 1);

		BoundingBox.reset(0, 0, 0);
		BoundingBox.addInternalBox(XMeshBuffer.BoundingBox);
		BoundingBox.addInternalBox(YMeshBuffer.BoundingBox);
		BoundingBox.addInternalBox(ZMeshBuffer.BoundingBox);
	}

	void setAxesScale(f32 scale) {
		ZMeshBuffer.Vertices.set_used(8);
		ZMeshBuffer.Material.Wireframe = false;
		ZMeshBuffer.Material.Lighting = false;
		ZMeshBuffer.Vertices[0] = video::S3DVertex(-0.25, -0.25, 0, -1, -1, -1, ZColor, 0, 1);
		ZMeshBuffer.Vertices[1] = video::S3DVertex(0.25, -0.25, 0, 1, -1, -1, ZColor, 1, 1);
		ZMeshBuffer.Vertices[2] = video::S3DVertex(0.25, 0.25, 0, 1, 1, -1, ZColor, 1, 0);
		ZMeshBuffer.Vertices[3] = video::S3DVertex(-0.25, 0.25, 0, -1, 1, -1, ZColor, 0, 0);
		ZMeshBuffer.Vertices[4] = video::S3DVertex(0.25, -0.25, scale, 1, -1, 1, ZColor, 0, 1);
		ZMeshBuffer.Vertices[5] = video::S3DVertex(0.25, 0.25, scale, 1, 1, 1, ZColor, 0, 0);
		ZMeshBuffer.Vertices[6] = video::S3DVertex(-0.25, 0.25, scale, -1, 1, 1, ZColor, 1, 0);
		ZMeshBuffer.Vertices[7] = video::S3DVertex(-0.25, -0.25, scale, -1, -1, 1, ZColor, 1, 1);
		ZMeshBuffer.BoundingBox.reset(0, 0, 0);

		YMeshBuffer.Vertices.set_used(8);
		YMeshBuffer.Material.Wireframe = false;
		YMeshBuffer.Material.Lighting = false;
		YMeshBuffer.Vertices[0] = video::S3DVertex(-0.25, 0, 0.25, -1, -1, -1, YColor, 0, 1);
		YMeshBuffer.Vertices[1] = video::S3DVertex(0.25, 0, 0.25, 1, -1, -1, YColor, 1, 1);
		YMeshBuffer.Vertices[2] = video::S3DVertex(0.25, 0, -0.25, 1, 1, -1, YColor, 1, 0);
		YMeshBuffer.Vertices[3] = video::S3DVertex(-0.25, 0, -0.25, -1, 1, -1, YColor, 0, 0);
		YMeshBuffer.Vertices[4] = video::S3DVertex(0.25, scale, 0.25, 1, -1, 1, YColor, 0, 1);
		YMeshBuffer.Vertices[5] = video::S3DVertex(0.25, scale, -0.25, 1, 1, 1, YColor, 0, 0);
		YMeshBuffer.Vertices[6] = video::S3DVertex(-0.25, scale, -0.25, -1, 1, 1, YColor, 1, 0);
		YMeshBuffer.Vertices[7] = video::S3DVertex(-0.25, scale, 0.25, -1, -1, 1, YColor, 1, 1);
		YMeshBuffer.BoundingBox.reset(0, 0, 0);

		XMeshBuffer.Vertices.set_used(8);
		XMeshBuffer.Material.Wireframe = false;
		XMeshBuffer.Material.Lighting = false;
		XMeshBuffer.Vertices[0] = video::S3DVertex(0, -0.25, 0.25, -1, -1, -1, XColor, 0, 1);
		XMeshBuffer.Vertices[1] = video::S3DVertex(0, -0.25, -0.25, 1, -1, -1, XColor, 1, 1);
		XMeshBuffer.Vertices[2] = video::S3DVertex(0, 0.25, -0.25, 1, 1, -1, XColor, 1, 0);
		XMeshBuffer.Vertices[3] = video::S3DVertex(0, 0.25, 0.25, -1, 1, -1, XColor, 0, 0);
		XMeshBuffer.Vertices[4] = video::S3DVertex(scale, -0.25, -0.25, 1, -1, 1, XColor, 0, 1);
		XMeshBuffer.Vertices[5] = video::S3DVertex(scale, 0.25, -0.25, 1, 1, 1, XColor, 0, 0);
		XMeshBuffer.Vertices[6] = video::S3DVertex(scale, 0.25, 0.25, -1, 1, 1, XColor, 1, 0);
		XMeshBuffer.Vertices[7] = video::S3DVertex(scale, -0.25, 0.25, -1, -1, 1, XColor, 1, 1);
		XMeshBuffer.BoundingBox.reset(0, 0, 0);
	}
};

#endif // ROOT_AXISSCENENODE_H
