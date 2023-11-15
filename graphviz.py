from graphviz import Source

# 读取文件
with open('tree', 'r') as file:
    graph_data = file.read()

# 创建图形
graph = Source(graph_data)

# 渲染并保存图形
graph.render(filename='output1', format='png')