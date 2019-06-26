class Tree_element{
	constructor(name, parent){
		this.name = name;
		this.parent = parent;
		this.children = [];
	}
	
	add_child(element){
		this.children.push(element);
	}
}

class Tree{
	constructor(){
		this.elements = [];
		this.roots = [];
		this.container = null;
	}

    set_container(html_node) {
        this.container = html_node;
    }
	
	add(name, parent_name){
		const new_element = new Tree_element(name, parent_name);
		this.elements.push(new_element);
		
		// find parent
		const parent = this.elements.find(el => el.name == new_element.parent);
		if(parent){
			parent.add_child(new_element);
		}
		else if(!new_element.parent){
			this.roots.push(new_element);
		}
		
		// find children
		const children = this.elements.filter(el => el.parent == new_element.name);
		for(let x of children){
			new_element.add_child(x);
		}
	}
	
	display() {
	    while (this.container.firstChild) {
	        this.container.removeChild(this.container.firstChild);
	    }

		for(let root of this.roots){
			let root_ul = document.createElement("ul");
			this.display_element(root_ul, root);
			this.container.appendChild(root_ul);
		}
	}
	
	display_element(html_node, element){
		let node = document.createTextNode(element.name);
		let span = document.createElement("span");
		span.appendChild(node);
		let li = document.createElement("li");
		li.appendChild(span);
		
		if(element.children.length > 0){
			let ul = document.createElement("ul");
			for(let child of element.children){
				this.display_element(ul, child);
			}
			li.appendChild(ul);
		}
		html_node.appendChild(li);
	}
}

function from_table_to_tree(table){
	let tree = new Tree();
	for(let entry of table){
		tree.add(entry.tag, entry.parent);
	}
	return tree;
}

function from_tree_to_table(tree){
	let table = [];
	for(let element of tree.elements){
		let entry = {
			tag : element.name,
			parent : element.parent
		};
		table.push(entry);
	}
	return table;
}

function test(){
	let x = new Tree();
	x.add("a", null);
	x.add("b", "a");
	x.add("c", "a");
	x.add("d", "b");
	x.add("e", "b");
	x.add("f", "c");
	x.add("g", "c");
	
	x.add("X", null);
	x.add("Y", "X");
	x.add("Z", "X");
    
	x.set_container(document.getElementById('tags_tree_container'));
	x.display();
}

app.controller('edit_tags', function ($scope, $http) {
    $scope.load_tags = function () {
		
        $http({
            // remote
            //url: "http://lukboz.000webhostapp.com/insert.php",

            // localhost
            url: "edit_tags.php",

            method: "GET",
            params: {
                date: new Date().getTime(),
                content: "tags"
            }
        }).then(function (response) {
			let tree = from_table_to_tree(response.data);
			let container = document.getElementById('tags_tree_container');
			tree.set_container(container);
			tree.display();
			
            $scope.error_info = "";
        }, function (response) {
			$scope.error_info = read_error_msg(response);
        });
    }
	
});
