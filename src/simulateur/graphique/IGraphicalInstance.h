//
// Created by louis on 16/11/17.
//

#ifndef ROOT_IGRAPHICALBEHAVIOR_H
#define ROOT_IGRAPHICALBEHAVIOR_H

#include <Color.h>
#include <MathToolbox/MathToolbox.h>

#include "../core/IBehavior.h"

class IGraphicalInstance : public IBehavior {
public:
	virtual ~IGraphicalInstance() = default;

	virtual int getId() const = 0;

	virtual void setScale(const Vector3f& scale) = 0;

	virtual Color3f getColor() = 0;
	virtual void setColor(const Color3f& color) = 0;
};

#endif // ROOT_IGRAPHICALBEHAVIOR_H
