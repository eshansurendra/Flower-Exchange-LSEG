# csv_processor/views.py
from django.http import JsonResponse, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.files.storage import FileSystemStorage
import subprocess
import os

from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.files.storage import default_storage
from django.core.files.base import ContentFile
import os

@csrf_exempt
def upload_csv(request):
    if request.method == 'POST':
        # Check if the file is in the request
        if 'file' not in request.FILES:
            return JsonResponse({'error': 'No file provided.'}, status=400)

        csv_file = request.FILES['file']

        if not csv_file.name.endswith('.csv'):
            return JsonResponse({'error': 'Invalid file type. Please upload a CSV file.'}, status=400)

        try:
            # Define the fixed file name and full path
            file_name = 'input_report.csv'
            file_path = os.path.join(default_storage.location, file_name)

            # Save or overwrite the file
            if default_storage.exists(file_name):
                default_storage.delete(file_name)
            
            saved_file_path = default_storage.save(file_name, ContentFile(csv_file.read()))

            # Print absolute file name of the saved file for debugging
            print(f"File successfully uploaded and saved at: {default_storage.location}/{saved_file_path}")

            exe_path = r'C:\Users\ASUS\Documents\LSEG-Flower-Exchange-App-Front\backend\app\output_program.exe'

            # Check if the executable exists
            if not os.path.exists(exe_path):
                print(f"Executable not found at: {exe_path}")
                return JsonResponse({'error': f'Executable not found at {exe_path}'}, status=500)

            # Run the executable with the saved file
            subprocess.run([exe_path, file_path])
            


            # Check if the output file was created(C:\Users\ASUS\Documents\LSEG-Flower-Exchange-App-Front\backend\execution_rep.csv)
            if not os.path.exists(r'C:\Users\ASUS\Documents\LSEG-Flower-Exchange-App-Front\backend\execution_rep.csv'):
                print(f"Output file not found.")
                return JsonResponse({'error': 'Output file not found.'}, status=500)
            
            # Return a success message and output file as a response
            with open(r'C:\Users\ASUS\Documents\LSEG-Flower-Exchange-App-Front\backend\execution_rep.csv', 'rb') as f:
                response = HttpResponse(f.read(), content_type='text/csv')
                response['Content-Disposition'] = 'attachment; filename="input_report.csv"'
                return response

        except Exception as e:
            print(f"Error during file upload or processing: {e}")
            return JsonResponse({'error': str(e)}, status=500)

    return JsonResponse({'error': 'Invalid request method.'}, status=405)


#make a test view
def test(request):
    return JsonResponse({'message': 'Hello, world!'})

from django.http import FileResponse

def get_response_csv(request):
    file_path = r'C:\Users\ASUS\Documents\GitHub\Flower-Exchange-LSEG\backend\execution_rep.csv'
    if not os.path.exists(file_path):
        return JsonResponse({'error': 'File not found.'}, status=404)
    return FileResponse(open(file_path, 'rb'), content_type='text/csv', as_attachment=True, filename=file_path)
