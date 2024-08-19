// 假设这些函数已经被定义  
double neuron_output(double input, double *weights, int weight_length, double bias) {
    celljs()
}
void update_weights_and_bias(double **weights, double *bias, double *grad_w, double grad_b, int length, double learning_rate);  
  
void cellgroupupdate(struct cell **array, int rows[], int numRows, double *true_out, double learning_rate) {  
    double *output_layer = runcellgroup(array, rows, numRows, NULL); // 假设输入为NULL，因为已经初始化输入层  
    // 从输出层开始反向传播  
    for (int i = numRows - 1; i >= 0; i--) {  
        int num_neurons = rows[i];  
        for (int j = 0; j < num_neurons; j++) {  
            struct cell *neuron = &((*array)[i][j]);  
  
            // 如果是输出层，计算损失函数的梯度  
            if (i == numRows - 1) {  
                double target = true_out[j];  
                double output = neuron->out;  
                double loss_grad = mse_loss(target, output); // 注意：这里应该是梯度，但mse_loss直接给出损失值，需要修改  
                double grad_output = 2.0 * (target - output); // 真实的损失函数关于输出的梯度  
  
                // 计算梯度关于权重的部分（这里简化为只考虑前一层）  
                for (int k = 0; k < neuron->wlong; k++) {  
                    double input_to_neuron = (*array)[i-1][k].out; // 假设这是前一层对应神经元的输出  
                    double grad_w = neuron->derivative_relu * grad_output * input_to_neuron;  
                    neuron->td_ptr[k] = grad_w; // 存储梯度到td_ptr中  
                }  
  
                // 计算梯度关于偏置的部分  
                double grad_b = neuron->derivative_relu * grad_output;  
                neuron->td_ptr[neuron->wlong] = grad_b; // 假设偏置的梯度也存储在td_ptr的末尾  
  
                // 更新权重和偏置（这里简化为只考虑前一层）  
                update_weights_and_bias(&neuron->w_ptr, &neuron->b, neuron->td_ptr, neuron->td_ptr[neuron->wlong], neuron->wlong + 1, learning_rate);  
            }  
            // 如果不是输出层，需要继续向前传播梯度（这里省略了具体实现，因为依赖于具体的网络结构）  
  
            // 注意：这里的梯度传播和更新是简化的，并且假设了网络是全连接的  
            // 在实际应用中，你可能需要处理不同类型的层（如卷积层、池化层等）和更复杂的连接模式  
        }  
    }  
  
    // 释放输出层分配的内存（如果需要）  
    free(output_layer);  
}  
  
// 注意：这里的update_weights_and_bias和neuron_output函数需要根据你的具体实现进行定义  
// update_weights_and_bias可能需要对每个权重和偏置分别调用某种形式的update_parameter函数  
// neuron_output函数需要根据神经元的输入、权重和偏置计算输出