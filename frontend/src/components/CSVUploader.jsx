import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import axios from 'axios';
import './CSVUploader.css'; // Import the CSS file

const CSVUploader = () => {
    const [file, setFile] = useState(null);
    const [error, setError] = useState('');
    const [responseMessage, setResponseMessage] = useState('');
    const [loading, setLoading] = useState(false);
    const navigate = useNavigate();

    const handleFileChange = (event) => {
        setFile(event.target.files[0]);
        setError(''); // Reset error message when a new file is selected
        setResponseMessage(''); // Reset response message when a new file is selected
    };

    const handleSubmit = async (event) => {
        event.preventDefault();

        if (!file) {
            setError('Please select a CSV file to upload.');
            return;
        }

        setLoading(true); // Set loading state
        const formData = new FormData();
        formData.append('file', file);

        try {
            const response = await axios.post('http://127.0.0.1:8000/upload/', formData, {
                headers: {
                    'Content-Type': 'multipart/form-data',
                },
            });
            console.log('File processed:', response.data);
            setResponseMessage(response.data.message || 'File uploaded successfully!');
            navigate('/view-csv'); // Redirect to the CSV viewer page
        } catch (error) {
            console.error('Error uploading file:', error);
            setError(
                error.response && error.response.data.error 
                    ? error.response.data.error 
                    : 'Failed to upload the file. Please try again.'
            );
        } finally {
            setLoading(false); // Reset loading state
        }
    };

    return (
        <div className="csv-uploader">
            <h1>Flower Exchange App Trader Application</h1>
            <form onSubmit={handleSubmit} className="upload-form">
                <input
                    type="file"
                    accept=".csv"
                    onChange={handleFileChange}
                    required
                    className="file-input"
                />
                <button type="submit" disabled={loading} className="upload-button">
                    {loading ? 'Uploading...' : 'Upload CSV'}
                </button>
            </form>
            {error && <p className="error-message">{error}</p>}
            {responseMessage && <p className="success-message">{responseMessage}</p>}
        </div>
    );
};

export default CSVUploader;
