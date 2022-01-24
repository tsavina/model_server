import argparse
from flask import Flask,render_template,request
from utils import predict_bert

parser = argparse.ArgumentParser(description='Get information about the status of served models over gRPC interace')
parser.add_argument('--ovms_address', required=False, default='localhost',
                    help='Specify url to grpc service. default:localhost')
parser.add_argument('--ovms_port', required=False, default=9000, type=int,
                    help='Specify port to grpc service. default: 9000')
args = vars(parser.parse_args())

# configuration
ovms_address = args.get('ovms_address')   # default='localhost'
ovms_port = args.get('ovms_port')  # default=9000

app = Flask(__name__)

@app.route('/search')
def search():
    return render_template('form.html')
 
@app.route('/result/', methods = ['POST', 'GET'])
def result():
    if request.method == 'GET':
        return 'The URL /result is accessed directly. Try going to /search to submit form'
    if request.method == 'POST':
        form_data = request.form
        answer = predict_bert(form_data["source"], form_data["search"], ovms_address, ovms_port)
        return render_template('data.html',form_data=form_data, answer=answer)
 
app.run(host='0.0.0.0', port=5000)
