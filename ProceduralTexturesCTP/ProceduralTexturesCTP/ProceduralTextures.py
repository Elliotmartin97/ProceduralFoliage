
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.gridspec as gridspec
import os, time
import PIL as pillow
import sys as sys

from keras.preprocessing.image import load_img
from keras.preprocessing.image import img_to_array
from keras.backend.tensorflow_backend import set_session
from keras import layers,models
from keras.optimizers import Adam
from keras.models import model_from_json
from keras.engine.saving import load_model
from keras.backend import resize_images
from tensorflow._api.v1 import image

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"

save_models = False
load_models = True
transparency = True
dir_data = "leaves"

n_train = 40;
n_test = 4;
name_imgs = np.sort(os.listdir("data/" + dir_data + "/"))
name_imgs_train = name_imgs[:n_train]
name_imgs_test = name_imgs[n_train:n_train + n_test]
img_shape = (64, 64, 3)

def get_npdata(name_imgs_train):
    x_train = []
    for i, id in enumerate(name_imgs_train):
        image = load_img("data/" + dir_data + "/" + id, target_size=img_shape[:2])
        image = img_to_array(image) / 255.0
        x_train.append(image)
    x_train = np.array(x_train)
    return(x_train)

x_train = get_npdata(name_imgs_train)
print("x_train.shape = {}".format(x_train.shape))

x_test = get_npdata(name_imgs_test)
print("x_test.shape = {}".format(x_test.shape))

optimizer_g = Adam(0.00007, 0.5)
optimizer_d = Adam(0.00007, 0.5)

##batch normalisation increases the speed of training
def conv_transpose_batch_leaky(layer, filter, kernel, strides, train, alpha):
    x = layers.Conv2DTranspose(filters=filter, kernel_size=kernel ,  strides=strides , use_bias=False)(layer)
    x = layers.BatchNormalization()(x, training=train)
    x = layers.LeakyReLU(alpha)(x)
    return x

def generator(img_shape, noise_shape = (100,), train= False):
    
    ##alpha
    alpha = 0.1

    ##kernel size
    k = 2
    ## latent variable as input
    input_noise = layers.Input(shape=noise_shape) 
    d = layers.Dense(1024, activation="relu")(input_noise) 
    d = layers.Dense(1024, activation="relu")(input_noise) 
    d = layers.Dense(128*8*8, activation="relu")(d)
    d = layers.Reshape((8,8,128))(d)
    d = layers.BatchNormalization()(d, training=train)
    d = layers.LeakyReLU(alpha)(d)

    d = conv_transpose_batch_leaky(layer=d, filter=256 ,kernel=k, strides=(2,2), train=train, alpha=alpha)
    d = layers.Conv2D( 128  , ( 1 , 1 ) , activation='relu' , padding='same', name='block_4')(d) ## 256


    d = conv_transpose_batch_leaky(layer=d, filter=128 ,kernel=k, strides=(2,2), train=train, alpha=alpha)
    d = layers.Conv2D( 64  , ( 1 , 1 ) , activation='relu' , padding='same', name='block_5')(d) ## 128
    
    d = conv_transpose_batch_leaky(layer=d, filter=64 ,kernel=k, strides=(2,2), train=train, alpha=alpha)
    d = layers.Conv2D( 32 , ( 1 , 1 ) , activation='relu' , padding='same', name='block_6')(d) ## 64

    img = layers.Conv2D( 3 , ( 1 , 1 ) , activation='sigmoid' , padding='same', name='final_block')(d)
    model = models.Model(input_noise, img)
    model.summary() 
    return(model)

noise_shape = (100,)

##load existing model or generate a new one
if load_models == True:

    with open("./models/" + dir_data + "/model_g.json", "r") as json_file:
        generator = model_from_json(json_file.read())

    generator.load_weights("./models/" + dir_data + "/model_g.h5")
else:
    generator = generator(img_shape, noise_shape = noise_shape)
    
generator.compile(loss='binary_crossentropy', optimizer=optimizer_g)

def get_noise(nsample=1, nlatent_dim=100):
    noise = np.random.normal(0, 1, (nsample,nlatent_dim))
    return(noise)

def plot_images(noise,path_save=None,titleadd=""):
    imgs = generator.predict(noise)
    fig = plt.figure(figsize=(10,5))
    for i, img in enumerate(imgs):
        ax = fig.add_subplot(1,nsample,i+1)
        ax.imshow(img)
    fig.suptitle("Generated images "+titleadd,fontsize=30)
    
    if path_save is not None:
        plt.savefig(path_save,
                    bbox_inches='tight',
                    pad_inches=0)
        plt.close()
    else:
        plt.show()

nsample = 1
noise = get_noise(nsample=nsample, nlatent_dim=noise_shape[0])

def build_discriminator(img_shape,noutput=1):

    ## x = resize_images(shape,2,2, data_format='channels_last',interpolation='nearest')

    input_img = layers.Input(shape=img_shape)

    x = layers.Conv2D(32, (3, 3), activation='relu', padding='same', name='block1_conv1')(input_img)
    x = layers.Conv2D(32, (3, 3), activation='relu', padding='same', name='block1_conv2')(x)
    x = layers.MaxPooling2D((2, 2), strides=(2, 2), name='block1_pool')(x)
    
    x = layers.Conv2D(64, (3, 3), activation='relu', padding='same', name='block2_conv1')(x)
    x = layers.Conv2D(64, (3, 3), activation='relu', padding='same', name='block2_conv2')(x)
    x = layers.MaxPooling2D((2, 2), strides=(2, 2), name='block2_pool')(x)

    x = layers.Conv2D(128, (3, 3), activation='relu', padding='same', name='block4_conv1')(x)
    x = layers.Conv2D(128, (3, 3), activation='relu', padding='same', name='block4_conv2')(x)
    x = layers.MaxPooling2D((2, 2), strides=(2, 2), name='block4_pool')(x)

    x = layers.MaxPooling2D((2, 2), strides=(1, 1), name='block5_pool')(x)

    
    x         = layers.Flatten()(x)
    x         = layers.Dense(1024,      activation="relu")(x)
    ##x         = layers.GaussianDropout(0.1)(x)
    out       = layers.Dense(noutput,   activation='sigmoid')(x)
    model     = models.Model(input_img, out)
    
    return model

##generate or load model
if load_models == True:

    with open("./models/" + dir_data + "/model_d.json", "r") as json_file:
        discriminator = model_from_json(json_file.read())

    discriminator.load_weights("./models/" + dir_data + "/model_d.h5")
else:
    discriminator = build_discriminator(img_shape)

discriminator.compile(loss = 'binary_crossentropy', optimizer = optimizer_d, metrics = ['accuracy'])

z = layers.Input(shape=noise_shape)
img = generator(z)

# For the combined model we will only train the generator
discriminator.trainable = False

# The valid takes generated images as input and determines validity
valid = discriminator(img)

# The combined model  (stacked generator and discriminator) takes
# noise as input => generates images => determines validity 
combined = models.Model(z, valid)
combined.compile(loss='binary_crossentropy', optimizer=optimizer_g)

def train(models, x_train, noise_plot, dir_result="/result/", epochs=10000, batch_size=16):
        '''
        models     : tuple containins three tensors, (combined, discriminator, generator)
        X_train    : np.array containing images (Nsample, height, width, Nchannels)
        noise_plot : np.array of size (Nrandom_sample_to_plot, hidden unit length)
        dir_result : the location where the generated plots for noise_plot are saved 
        
        '''
        combined, discriminator, generator = models
        nlatent_dim = noise_plot.shape[1]
        batch  = int(batch_size)
        history = []
        train_switch = False

        ##get first discriminator value
        idx = np.random.randint(0, x_train.shape[0], batch)
        imgs = x_train[idx]
        noise = get_noise(batch, nlatent_dim)
        gen_imgs = generator.predict(noise, True)

        d_loss_real = discriminator.train_on_batch(imgs, np.ones((batch, 1)))
        d_loss_fake = discriminator.train_on_batch(gen_imgs, np.zeros((batch, 1)))
        d_loss = 0.5 * np.add(d_loss_real, d_loss_fake)

        for epoch in range(epochs):

            # ---------------------
            #  Train Discriminator
            # ---------------------

            # Select a random batch of images
            idx = np.random.randint(0, x_train.shape[0], batch)
            imgs = x_train[idx]
            noise = get_noise(batch, nlatent_dim)

            # Generate a batch of new images
            gen_imgs = generator.predict(noise, True)

            
            # Train the discriminator 
            d_loss_real = discriminator.train_on_batch(imgs, np.ones((batch, 1)))
            d_loss_fake = discriminator.train_on_batch(gen_imgs, np.zeros((batch, 1)))
            d_loss = 0.5 * np.add(d_loss_real, d_loss_fake)


            # ---------------------
            #  Train Generator
            # ---------------------

            noise = get_noise(batch_size, nlatent_dim)

            # The generator wants the discriminator to label the generated samples
            # as valid (ones)
            valid_y = (np.array([1] * batch_size)).reshape(batch_size,1)

            g_loss = combined.train_on_batch(noise, valid_y)

            history.append({"D":d_loss[0],"G":g_loss})
            
            g_loss_f = g_loss
            d_loss_f = d_loss[0]

            if epoch % 100 == 0:
                # Plot the progress
                print ("Epoch {:05.0f} [D loss: {:4.3f}, acc.: {:05.1f}%] [G loss: {:4.3f}]".format(
                    epoch, d_loss[0], 100*d_loss[1], g_loss))
            if epoch % int(epochs/100) == 0:
                plot_images(noise_plot,
                                      path_save=dir_result+"/image_{:05.0f}.png".format(epoch),
                                      titleadd="Epoch {}".format(epoch))
                        
        return(history)
    ##generate a new image, then write as png, then use pillow to convert to tga ready to use as a texture for DirectX
def generate_new(noise):
    sess = tf.Session()
    img = generator.predict(noise, True)
    img = img * 255
    img_rand = img[0]
    img_rand = np.squeeze(img_rand)
    img_rand = tf.convert_to_tensor(img_rand, np.uint8)
    image_file = image.encode_png(img_rand)
    if(transparency == True):
        file_name = tf.constant("./export/leaf.png")
        file = tf.write_file(file_name,image_file)
        sess.run(file)
        im = pillow.Image.open("./export/leaf.png")
       ## im = im.resize((640,640), pillow.Image.NEAREST)
        im = im.convert("RGBA")
        datas = im.getdata()
        newData = []
        for item in datas:
             if item[0] >= 200 and item[1] >= 200 and item[2] >= 200:
                newData.append((0, 0, 0, 0))
             else:
                newData.append(item)

        im.putdata(newData)
        im.save("C:/Users/Ellio/source/repos/ProceduralFoliage/ProceduralFoliage/Engine/Textures/leaf.tga")
    else:
        file_name = tf.constant("./export/branch.png")
        file = tf.write_file(file_name,image_file)
        sess.run(file)
        im = pillow.Image.open("./export/branch.png")
        im = im.convert("RGBA")
        ##im = im.resize((640,640), pillow.Image.NEAREST)
        im.save("C:/Users/Ellio/source/repos/ProceduralFoliage/ProceduralFoliage/Engine/Textures/branch.tga")

    print("Image Saved")

def save_model():
     ## save discriminator model
    model_d = _models[1]
    model_json = model_d.to_json()
    with open("./models/" + dir_data + "/model_d.json", "w") as json_file:
        json_file.write(model_json)

    model_d.save_weights("./models/" + dir_data + "/model_d.h5")
    print("Saved Discriminator")

    ##save generator model
    model_g = _models[2]
    model_json = model_g.to_json()
    with open("./models/" + dir_data + "/model_g.json", "w") as json_file:
        json_file.write(model_json)

    model_g.save_weights("./models/" + dir_data + "/model_g.h5")
    print("Saved Generator")

dir_result="./result_GAN/"

try:
    os.mkdir(dir_result)
except:
    pass
    
_models = combined, discriminator, generator          

if(save_models):
    history = train(_models, x_train, noise, dir_result=dir_result,epochs=2000, batch_size=8)
    save_model()
else:
    generate_new(noise)
