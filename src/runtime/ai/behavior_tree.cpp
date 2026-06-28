/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "behavior_tree.h"
#include "core/filearchive.h"

#include <nlohmann/json.hpp>

using json = ::nlohmann::json;

namespace segfault::ai {

	using namespace segfault::core;

	void BehaviorTreeNode::addChild(BehaviorTreeNode* node) {
		// Implementation for adding a child node
	}

	class BehaviorTreeNodeFactory {
	public:
		BehaviorTreeNodeFactory() = default;
		~BehaviorTreeNodeFactory() = default;

		static BehaviorTreeNode* createNode(const json& nodeData) {
			if (!nodeData.contains("type")) {
				return nullptr;
			}

			std::string nodeType = nodeData["type"].get<std::string>();

			// Create the appropriate node based on the type
			if (nodeType == "Sequence") {
				return new SequenceNode(nodeData);
			} else if (nodeType == "Selector") {
				return new SelectorNode(nodeData);
			} else if (nodeType == "Action") {
				return new ActionNode(nodeData);
			} else if (nodeType == "Condition") {
				return new ConditionNode(nodeData);
			} else {
				// Unknown node type
				return nullptr;
			}
		}
	};

	BehaviorTree::BehaviorTree() {
		// Constructor implementation (if needed)
	}

	BehaviorTree::~BehaviorTree() {
		// Destructor implementation (if needed)
	}

	bool BehaviorTree::init(const char* configFile) {
		if (configFile == nullptr) {
			return false;
		}
	
		FileArchive archive(configFile, "r", true, false);
		if (!archive.isValid()) {
			return false;
		}

		// Read the entire file into a string
		size_t size = archive.getSize();
		std::string content(size, '\0');
		archive.read(reinterpret_cast<uint8_t*>(&content[0]), size);
		json Doc{ json::parse(content) };

		mRootNode = BehaviorTreeNodeFactory::createNode(Doc);
		if (mRootNode == nullptr) {	
			return false;
		}

		return true;
	}

	void BehaviorTree::update() {
		if (mRootNode == nullptr) {
			return;
		}
		
		mRootNode->tick();
	}

} // namespace segfault::ai
