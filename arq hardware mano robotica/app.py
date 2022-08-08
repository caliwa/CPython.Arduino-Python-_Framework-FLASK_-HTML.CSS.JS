from flask import Flask, render_template, Response, request
import serial, time

serialcom = serial.Serial('COM3', 9600)
serialcom.timeout = 1

app = Flask(__name__)

def brazoArriba():
    serialcom.write(str('brazoArriba').encode())

def brazoAbajo():
    serialcom.write(str('brazoAbajo').encode())

def brazoDerecha():
    serialcom.write(str('brazoDerecha').encode())

def brazoIzquierda():
    serialcom.write(str('brazoIzquierda').encode())

def Agarre():
    serialcom.write(str('Agarre').encode())

def Bajar():
    serialcom.write(str('Bajar').encode())

def Vidas():
    serialcom.write(str("RestarVida").encode())

def Reintentar():
    serialcom.write(str('Reintentar').encode())

def Continuar():
    serialcom.write(str('Continuar').encode())

def Lanzar():
    mandar = 0
    tiempo1 = round(time.time())
    tiempo2 = 0
    arduino_datosUltraSonido = 0
    while tiempo2 - tiempo1 != 4:
        tiempo2 = round(time.time())
        print(tiempo2 - tiempo1)
        arduino_datosUltraSonido = serialcom.readline().decode('utf8')
        print("TRAE: "+arduino_datosUltraSonido)

        arduino_datosUltraSonido = arduino_datosUltraSonido[:7]

        if tiempo2 - tiempo1 >= 1.5:
            if arduino_datosUltraSonido != '':
                if float(arduino_datosUltraSonido) != 0:
                    if float(arduino_datosUltraSonido) < 12:
                        mandar = 1
                        break

    return mandar

def disconnect():
    serialcom.close()

cont = 0
centHTML = 0
auxCont = 0
gameOver = 3
botonAvanzar = 0
botonLanzar = 0
centSubida = 0
centLanzar = 0
centAgarre = 0
band = False

@app.route("/", methods=['GET', 'POST'])
def index():
    global cont, centHTML, gameOver, auxCont, botonAvanzar, botonLanzar, centSubida, centLanzar, centAgarre, band

    tiempo1 = round(time.time())
    tiempo2 = 0
    
    if request.method == 'POST':
        
        if 'continuar' in request.form.to_dict():
            Continuar()
            centHTML = 1

        if 'reintentar' in request.form.to_dict():
            centHTML = 0
            cont = 0
            gameOver = 3
            Reintentar()

        if 'lanzar' in request.form.to_dict():
            if centAgarre == 1:
                serialcom.write(str('Lanzar').encode())
                auxCont = Lanzar()
                centAgarre = 0
                cont += auxCont
                if auxCont == 0:
                    gameOver -= 1
                    Vidas()

        if gameOver == 0:
            centHTML = 2

        if 'bajar' in request.form.to_dict():
            Bajar()
            band = True
            
            if centSubida == 0:
                botonAvanzar = 1
                centSubida = 1
                
            elif centSubida == 1:
                botonAvanzar = 0
                centSubida = 0

        if 'agarre' in request.form.to_dict():
            Agarre()
            if centAgarre == 0:
                centAgarre = 1
            elif centAgarre == 1:
                centAgarre = 0

        if 'barrb' in request.form.to_dict():
            brazoArriba()

        if 'babaj' in request.form.to_dict():
            brazoAbajo()

        if 'bizq' in request.form.to_dict():
            brazoIzquierda()

        if 'bder' in request.form.to_dict():
            brazoDerecha()
           
    if centSubida == 0 and centAgarre == 1:
        botonLanzar = 1
    else:
        botonLanzar = 0

    if band == True:
        band = False
        tiempo1 = round(time.time())
        tiempo2 = 0
        while tiempo2 - tiempo1 != 2:
            tiempo2 = round(time.time())

    return render_template('index.html', hola = cont, centHTML = centHTML, contadorVida = gameOver, avanzar = botonAvanzar, lanzar = botonLanzar)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000)