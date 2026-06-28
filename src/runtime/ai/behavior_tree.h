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
#pragma once

#include "core/segfault.h"

#include <functional>

namespace segfault::ai {
	enum class NodeStatus {
		INVALID = -1,
		IDLE = 0,
		RUNNING = 1,
		SUCCESS = 2,
		FAILURE = 3,
		SKIPPED = 4,
		COUNT
	};

	enum class NodeType {
		INVALID = -1,
		SEQUENCE,
		SELECTOR,
		CONDITION,
		ACTION,
		Count
	};

	//---------------------------------------------------------------------------------------------
	/// @class BehaviorTreeNode
	/// @brief The BehaviorTreeNode class represents a single node in a behavior tree, which is a
	/// hierarchical structure used to model the decision-making process of an AI agent. Each node
	/// can represent an action, condition, or control flow, allowing for complex behaviors to be
	/// defined in a modular and reusable way.
	//---------------------------------------------------------------------------------------------
	class BehaviorTreeNode {
	public:
		// No copying
		BehaviorTreeNode(const BehaviorTreeNode& rhs) = delete;
		BehaviorTreeNode& operator = (const BehaviorTreeNode& rhs) = delete;

		BehaviorTreeNode() = default;
		virtual ~BehaviorTreeNode() = default;
		
		/// @brief Pure virtual function to be implemented by derived classes
		virtual NodeStatus tick() = 0;

		virtual void addChild(BehaviorTreeNode* node) {}

		NodeType getNodeType() const { return mNodeType; }
		NodeStatus getNodeStatus() const { return mNodeStatus; }

	private:
		NodeType mNodeType{ NodeType::INVALID };
		NodeStatus mNodeStatus{ NodeStatus::INVALID };
	};

	class SequenceNode : public BehaviorTreeNode {
	public:
		NodeStatus tick() override {
			for (auto *child : mChildren) {
				assert(child != nullptr);
				auto status = child->tick();
				if (status != NodeStatus::SUCCESS) {
					return status;
				}
			}
			return NodeStatus::SUCCESS;
		}

		~SequenceNode() override {
			for (auto child : mChildren) {
				delete child;
			}
			mChildren.clear();
		}

		void addChild(BehaviorTreeNode* node) override {
			mChildren.push_back(node);
		}

	private:
		std::vector< BehaviorTreeNode*> mChildren;
	};

	class ConditionNode : public SequenceNode {
	public:
		explicit ConditionNode(std::function<bool()> func) : mFunc(func) {
			// empty
		}

		NodeStatus tick() override {
			if (mFunc()) {
				return mFunc() ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
			}
			
			return NodeStatus::FAILURE;
		}

	private:
		std::function<bool()> mFunc;
	};

	class ActionNode : public BehaviorTreeNode {
	public:
		explicit ActionNode(std::function<NodeStatus()> func) : mFunc(func) {
			// empty
		}

		NodeStatus tick() override {
			return mFunc();
		}

	private:
		std::function<NodeStatus()> mFunc;
	};

	class SelectorNode : public BehaviorTreeNode {
	public:
		~SelectorNode() override {
			for (auto* child : mChildren) {
				delete child;
			}
			mChildren.clear();
		}

		void addChild(BehaviorTreeNode* node) override {
			mChildren.push_back(node);
		}

		NodeStatus tick() override {
			for (auto* child : mChildren) {
				assert(child != nullptr);

				auto status = child->tick();
				if (status != NodeStatus::FAILURE) return status;
			}
			return NodeStatus::FAILURE;
		}

	private:
		std::vector< BehaviorTreeNode*> mChildren;
	};

	//---------------------------------------------------------------------------------------------
	/// @class BehaviorTree
	/// @brief The BehaviorTree class represents a behavior tree, which is a hierarchical structure 
	/// used to model the decision-making process of an AI agent. It consists of nodes that 
	/// represent actions, conditions, and control flow, allowing for complex behaviors to be 
	/// defined in a modular and reusable way.
	//---------------------------------------------------------------------------------------------
	class SEGFAULT_EXPORT BehaviorTree final {
	public:
		// No copying
		BehaviorTree(const BehaviorTree& rhs) = delete;
		BehaviorTree& operator = (const BehaviorTree& rhs) = delete;

		/// @brief The class constructor.
		BehaviorTree();

		/// @brief The class destructor.
		~BehaviorTree();

		/// @brief Initializes the behavior tree with the specified parameters.
		/// @param[ in ] configFile The path to the configuration file that defines the behavior tree structure.
		/// @return True if initialization was successful, false otherwise.
		bool init(const char* configFile);

		/// @brief Updates the behavior tree, processing the nodes and executing actions as needed.
		void update();

	private:
		BehaviorTreeNode* mRootNode{ nullptr };
	};

} // namespace segfault::ai
