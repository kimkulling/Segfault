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

namespace segfault::ai {
	enum class NodeStatus {
		IDLE = 0,
		RUNNING = 1,
		SUCCESS = 2,
		FAILURE = 3,
		SKIPPED = 4,
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
	};

	class LeafNode : public BehaviorTreeNode {
	public:
		NodeStatus tick() override {
			// Implementation of the leaf node's behavior
			return NodeStatus::IDLE;
		}
	};

	class SequenceNode : public BehaviorTreeNode {
	public:
		NodeStatus tick() override {
			// Implementation of the sequence node's behavior
			return NodeStatus::IDLE;
		}
	};

	class ConditionNode : public LeafNode {
	public:
		NodeStatus tick() override {
			// Implementation of the condition node's behavior
			return NodeStatus::IDLE;
		}
	};

	class ActionNode : public LeafNode {
	public:
		NodeStatus tick() override {
			// Implementation of the action node's behavior
			return NodeStatus::IDLE;
		}
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
